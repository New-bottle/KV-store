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
	FILE *block_fp = fopen(BLOCK_FILE_NAME, "wb+");
	File *tmp = new File(block_fp, BLOCK_PAGE_SIZE);
	node header = tmp->load(0);
	int *hd = (int*)header.p;
	hd[0] = BLOCK_PAGE_SIZE;
	hd[1] = block_page_cnt = 0;
	header.dirty_mark = true;
	tmp->release(0);
	tmp->flush(0);
	delete tmp;
	fclose(block_fp);
}

void DiskManager::create_filter_file() // init the header
{
	FILE *filter_fp = fopen(FILTER_FILE_NAME, "wb+");
	File *tmp = new File(filter_fp, FILTER_PAGE_SIZE);
	node header = tmp->load(0);
	int *hd = (int*)header.p;
	hd[0] = FILTER_PAGE_SIZE;
	hd[1] = filter_page_cnt = 0;
	header.dirty_mark = true;
	tmp->release(0);
	tmp->flush(0);
	delete tmp;
	fclose(filter_fp);
}

DiskManager::DiskManager()
{
	FILE *block_fp;
	FILE *filter_fp;
	if (!access(BLOCK_FILE_NAME, 0)) { // file not exist
		create_block_file();
	}
	block_fp = fopen(BLOCK_FILE_NAME, "rb+");
	block = new File(block_fp, BLOCK_PAGE_SIZE);
	node &header = block->load(0);
	int *tmp = (int*)header.p; 
	// tmp[0] = block_page_size
	// tmp[1] = block_page_cnt
	block_page_size = tmp[0];
	block_page_cnt = tmp[1];
	block->release(0);

	if (!access(FILTER_FILE_NAME, 0)) { // file not exist
		create_filter_file();
	}
	filter_fp = fopen(FILTER_FILE_NAME, "rb+");
	bloom_filter = new File(filter_fp, FILTER_PAGE_SIZE);
	node &header2 = bloom_filter->load(0);
	tmp = (int*) header2.p;
	// tmp[0] = filter_page_size
	// tmp[1] = filter_page_cnt
	filter_page_size = tmp[0];
	filter_page_cnt = tmp[1];
	bloom_filter->release(0);
	// open files
}

DiskManager::~DiskManager()
{
	if (block != nullptr) delete block;
	if (bloom_filter != nullptr) delete bloom_filter;
	// close files
}

template<typename value_type, int page_size>
bool DiskManager::add_page(int hash_code, Buffer<value_type, page_size>& buffer)
{
	/* 
	 * First find the first block of the hash_code,
	 * then try to insert the page. If failed, create
	 * a new block and insert from the head.
	 */
	int data[100]; // TODO: how big is enough?
	node &header = bloom_filter->load(0); // page[0] is the header
	int *head = ((int*)header.p) + 2; // head[i] = the first bloom_filter of hash_code=i

	//fseek(bloom_filter_fp, hash_code * sizeof(int) / 8, 0);
	//fread(data, sizeof(int), 1, bloom_filter_fp); // data[0] = head[hash_code]
}