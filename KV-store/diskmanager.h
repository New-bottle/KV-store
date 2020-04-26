#pragma once
#include "buffer.h"
#include "header.h"
#include "bloomfilter.h"
#include "file.h"
#include "datablock.h"
#include <cstdio>
#include <io.h>
#include <bitset>

template<typename value_type>
class DiskManager
{
private:
	File *block;
	File *bloom_filter;
public:
	int *block_page_cnt;
	int *filter_page_cnt;
	int *block_page_size;
	int *filter_page_size;

	void create_block_file()
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

	void create_filter_file()
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
			// 这里要小心，HASH_NUMBER过大的时候会超过第0个page所能存储的范围，会需要
			// 扩大FILTER_PAGE_SIZE，这不是一个好的设计，最好能单独设置header的size
		}
		header.dirty_mark = true;
		tmp.release(0);
		tmp.flush(0);
	}

#ifdef DEBUG
	unsigned int filter_cnt;
	unsigned int block_cnt;
#endif

private:
	void add_filter(int hash_code)
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
		*bf.block = *block_page_cnt + 1;
		DataBlock<value_type> db(block, *block_page_cnt + 1);
		db.clear();
		*block_page_cnt += BLOCK_PAGE_NUM;
		*bf.page_cnt = 0;
		bloom_filter->release(head[hash_code]);
		bloom_filter->release(0);
	}

	bool filter_full(int filter_num)
	{
		if (filter_num > *filter_page_cnt) throw "Filter num exceed";
		node &filter = bloom_filter->load(filter_num);
		BloomFilter bf(filter.p);
		bool sign = *bf.page_cnt == BLOCK_PAGE_SIZE / PAGE_SIZE;
		bloom_filter->release(filter_num);
		return sign;
	}

	BloomFilter load_filter(int filter_num)
	{
	#ifdef DEBUG
		++filter_cnt;
	#endif
		node &filter = bloom_filter->load(filter_num);
		return BloomFilter(filter.p);
	}

	DataBlock<value_type> load_block(int block_num)
	{
	#ifdef DEBUG
		++block_cnt;
	#endif
		return DataBlock<value_type>(block, block_num);
	}

public:
	DiskManager()
	{
	#ifdef DEBUG
		filter_cnt = block_cnt = 0;
	#endif
		FILE *block_fp;
		FILE *filter_fp;
		if (_access(BLOCK_FILE_NAME, 0)) { // file not exist
			create_block_file();
		}
		block_fp = fopen(BLOCK_FILE_NAME, "rb+");
		block = new File(block_fp, BLOCK_PAGE_SIZE);
		node &header = block->load(0);
		header.dirty_mark = true;
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
		header2.dirty_mark = true;
		tmp = (int*) header2.p;
		// tmp[0] = filter_page_size
		// tmp[1] = filter_page_cnt
		filter_page_size = tmp;
		filter_page_cnt = tmp + 1;
		//bloom_filter->release(0);
		// open files
	}

	//DiskManager(FILE *block_fp, int block_page_size, FILE *bloom_filter_fp, int bf_page_size);
	~DiskManager()
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

	void add_page(int hash_code, Buffer<value_type>& buffer)
	{
		/* 
		 * First find the first block of the hash_code,
		 * then try to insert the page. If failed, create
		 * a new block and insert from the head.
		 */
		node &header = bloom_filter->load(0); // page[0] is the header
		int *filter_head = ((int*)header.p) + 2; // head[i] = the first bloom_filter of hash_code=i
		if (filter_head[hash_code] == 0 ) {
			add_filter(hash_code);
		}
		int *i = (int*)buffer.p + 1;
		while (true) {
			bool flag = true;
			node &filter = bloom_filter->load(filter_head[hash_code]);
			BloomFilter bf(filter.p);

			DataBlock<value_type> data_block(block, *bf.block);
			value_type *j;
			for (; i < (int*)buffer.head; i += 2) { // 一个int存key，一个int存value头
				j = (value_type*)(buffer.p + *(i + 1));
				if (data_block.add_item(*i, *j)) {
					bf.insert(*i);
				}
				else { // add failed: block is full (maybe not)
					flag = false;
					bloom_filter->release(filter_head[hash_code]);
					add_filter(hash_code);
					break;
				}
				// 插入bloom filter
			}
			++(*bf.page_cnt); // TODO should be replaced or removed
			if (flag) {
				bloom_filter->release(filter_head[hash_code]);
				bloom_filter->release(0);
				break;
			}
		}
		buffer.clear();
	}


	BloomFilter load_first_filter(int hash_code)
	{
		node &header = bloom_filter->load(0);
		int *head = ((int*)header.p) + 2;
		return load_filter(head[hash_code]);
	}

	BloomFilter load_next_filter(const BloomFilter& cur_filter)
	{
		return load_filter(*cur_filter.next);
	}

	DataBlock<value_type> load_block(const BloomFilter& cur_filter)
	{
		return load_block(*cur_filter.block);
	}

	value_type* search(int key)
	{
		int hash_code = HashFunction::hash1(key);
		BloomFilter filter = load_first_filter(hash_code);
		while (true) {
			if (filter.contains(key)) {
				DataBlock<value_type> data_block = load_block(filter);
				value_type *ans = data_block.search(key);
				if (ans != nullptr) return ans;
			}
			if (*filter.next == 0)
				return nullptr; // not found
				//throw "Not Found!";
			filter = load_next_filter(filter);
		}
	}
};
