#include "pch.h"
#include "diskmanager.h"

/*
DiskManager::DiskManager()
{
	FILE *block_fp;
	FILE *filter_fp;
	int tmp[2];
	if (!access(BLOCK_FILE_NAME, 0)) { // file not exist
		block_fp = fopen(BLOCK_FILE_NAME, "wb+");
		block_page_cnt = 0;
		block_page_size = BLOCK_PAGE_SIZE;
		tmp[0] = block_page_size;
		tmp[1] = block_page_cnt;
		fwrite(tmp, sizeof(int), 2, block_fp);
	}
	else { // read & write
		block_fp = fopen(BLOCK_FILE_NAME, "rb+");
		fread(tmp, sizeof(int), 2, block_fp);
		block_page_size = tmp[0];
		block_page_cnt = tmp[1];
	}
	block = new File(block_fp, block_page_size);

	if (!access(FILTER_FILE_NAME, 0)) {
		filter_fp = fopen(FILTER_FILE_NAME, "wb+");
		filter_page_cnt = 0;
		filter_page_size = FILTER_PAGE_SIZE;
		tmp[0] = filter_page_size;
		tmp[1] = filter_page_cnt;
		fwrite(tmp, sizeof(int), 2, filter_fp);
	}
	else {
		filter_fp = fopen(FILTER_FILE_NAME, "rb+");
		fread(tmp, sizeof(int), 2, filter_fp);
		filter_page_size = tmp[0];
		filter_page_cnt = tmp[1];
	}
	bloom_filter = new File(filter_fp, filter_page_size);
	// open files
}
*/

void DiskManager::create_block_file() // init the header
{
	printf("Creating block file\n");
	FILE *block_fp = fopen(BLOCK_FILE_NAME, "wb+");
	File tmp(block_fp, BLOCK_PAGE_SIZE);
	node& header = tmp.load(0);
	int *hd = (int*)header.p;
	hd[0] = BLOCK_PAGE_SIZE;
	hd[1] = 0;
	header.dirty_mark = true;
	tmp.release(0);
	tmp.flush(0);
}

void DiskManager::create_filter_file() // init the header
{
	printf("Creating filter file\n");
	FILE *filter_fp = fopen(FILTER_FILE_NAME, "wb+");
	File tmp(filter_fp, FILTER_PAGE_SIZE);
	node& header = tmp.load(0);
	int *hd = (int*)header.p;
	hd[0] = FILTER_PAGE_SIZE;
	hd[1] = 0;
	for (int i = 0; i < HASH_NUMBER; ++i) {
		hd[2 + i] = 0;
	}
	header.dirty_mark = true;
	tmp.release(0);
	tmp.flush(0);
}

DiskManager::DiskManager()
{
	FILE *block_fp;
	FILE *filter_fp;
	if (_access(BLOCK_FILE_NAME, 0)) { // file not exist
		create_block_file();
	}
	block_fp = fopen(BLOCK_FILE_NAME, "rb+");
	block = new File(block_fp, BLOCK_PAGE_SIZE);
	node &header = block->load(0);
	int *tmp = (int*)header.p; 
	// tmp[0] = block_page_size
	// tmp[1] = block_page_cnt
	block_page_size = tmp;
	block_page_cnt = tmp + 1;
	//block->release(0); // 不能release，block_page_size/cnt这两个指针还需要使用这里的数据，析构时再release

	if (_access(FILTER_FILE_NAME, 0)) { // file not exist
		create_filter_file();
	}
	filter_fp = fopen(FILTER_FILE_NAME, "rb+");
	bloom_filter = new File(filter_fp, FILTER_PAGE_SIZE);
	node &header2 = bloom_filter->load(0);
	tmp = (int*) header2.p;
	// tmp[0] = filter_page_size
	// tmp[1] = filter_page_cnt
	filter_page_size = tmp;
	filter_page_cnt = tmp + 1;
	//bloom_filter->release(0);
	// open files
}

DiskManager::~DiskManager()
{
	if (block != nullptr) {
		//block->release(0);
		delete block;
	}
	if (bloom_filter != nullptr) {
		//bloom_filter->release(0);
		delete bloom_filter;
	}
	// close files
}

/*
template<typename value_type>
void DiskManager::add_page(int hash_code, Buffer<value_type>& buffer)
{
	/* 
	 * First find the first block of the hash_code,
	 * then try to insert the page. If failed, create
	 * a new block and insert from the head.
	 *
	node &header = bloom_filter->load(0); // page[0] is the header
	int *head = ((int*)header.p) + 2; // head[i] = the first bloom_filter of hash_code=i
	bool flag = false;
	if (head[hash_code] == 0 || filter_full(head[hash_code])) {
		add_filter(hash_code);
	}
	node &filter = bloom_filter->load(head[hash_code]);
	BloomFilter bf(filter.p);
	node &data_block = block->load(*bf.block);
	data_block.dirty_mark = true;
	DataBlock data(data_block.p);
	char *head = data_block[bf.page_cnt], tail = data_block[bf.page_cnt + 1];
	// TODO 插入bloom filter
	for (int *i = buffer.p; i < buffer.head; i += 2) { // 一个int存key，一个int存value头
		bf.insert(*i);
	}
	memcpy(data_block[*bf.page_cnt], buffer.p, PAGE_SIZE);
	++bf.page_cnt;
}
*/


void DiskManager::add_filter(int hash_code)
{
	node &header = bloom_filter->load(0);
	header.dirty_mark = true;
	int *head = ((int*)header.p) + 2;
	int next_filter = head[hash_code];
	head[hash_code] = ++(*filter_page_cnt);
	node &filter = bloom_filter->load(head[hash_code]);
	filter.dirty_mark = true;
	BloomFilter bf(filter.p);
	*bf.next = next_filter;
	*bf.block = ++(*block_page_cnt);
	*bf.page_cnt = 0;
	bloom_filter->release(head[hash_code]);
	bloom_filter->release(0);
}

bool DiskManager::filter_full(int filter_num)
{
	if (filter_num > *filter_page_cnt) throw "Filter num exceed";
	node &filter = bloom_filter->load(filter_num);
	BloomFilter bf(filter.p);
	bool sign = *bf.page_cnt == BLOCK_PAGE_SIZE / PAGE_SIZE;
	bloom_filter->release(filter_num);
	return sign;
}

BloomFilter DiskManager::load_filter(int filter_num)
{
	node &filter = bloom_filter->load(filter_num);
	return BloomFilter(filter.p);
}

DataBlock DiskManager::load_block(int block_num)
{
	node &data_block = block->load(block_num);
	return DataBlock(data_block.p);
}

BloomFilter DiskManager::load_first_filter(int hash_code) 
{
	node &header = bloom_filter->load(0);
	int *head = ((int*)header.p) + 2;
	return load_filter(head[hash_code]);
}

BloomFilter DiskManager::load_next_filter(const BloomFilter& cur_filter)
{
	return load_filter(*cur_filter.next);
}

DataBlock DiskManager::load_block(const BloomFilter& cur_filter)
{
	return load_block(*cur_filter.block);
}

void* DiskManager::search(int key)
{
	int hash_code = HashFunction::hash1(key);
	BloomFilter filter = load_first_filter(hash_code);
	while (true) {
		if (filter.contains(key)) {
			DataBlock data_block = load_block(filter);
			for (int i = 0; i < BLOCK_PAGE_SIZE / PAGE_SIZE; ++i) {
				int *cnt = (int*)data_block[i], *head = (int*)(data_block[i] + sizeof(int));
				void *value = nullptr;
				for (int j = 0; j < *cnt; ++j) {
					// head[j << 1]     -> key
					// head[j << 1 | 1] -> pos of value
					if (head[j << 1] == key) {
						value = data_block[i] + head[j << 1 | 1];
						return value;
					}
				}
			}
		}
		if (filter.next == 0)
			throw "Not Found!";
		filter = load_next_filter(filter);
	}
}
