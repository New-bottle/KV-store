#pragma once
#include "buffer.h"
#include "header.h"
#include "bloomfilter.h"
#include "file.h"
#include "datablock.h"
#include <cstdio>
#include <io.h>
#include <bitset>

class DiskManager
{
private:
	File *block;
	File *bloom_filter;
public:
	unsigned int *block_page_cnt;
	unsigned int *filter_page_cnt;
	unsigned int *block_page_size;
	unsigned int *filter_page_size;

	void create_block_file();
	void create_filter_file();
#ifdef DEBUG
	unsigned int filter_cnt;
	unsigned int block_cnt;
#endif

private:
	void add_filter(int hash_code);
	bool filter_full(int filter_num);
	BloomFilter load_filter(int filter_num);
	DataBlock load_block(int block_num);
public:
	DiskManager();
	//DiskManager(FILE *block_fp, int block_page_size, FILE *bloom_filter_fp, int bf_page_size);
	~DiskManager();
	template<typename value_type>
	void add_page(int hash_code, Buffer<value_type>& buffer)
	{
		/* 
		 * First find the first block of the hash_code,
		 * then try to insert the page. If failed, create
		 * a new block and insert from the head.
		 */
		node &header = bloom_filter->load(0); // page[0] is the header
		int *filter_head = ((int*)header.p) + 2; // head[i] = the first bloom_filter of hash_code=i
		bool flag = false;
		if (filter_head[hash_code] == 0 || filter_full(filter_head[hash_code])) {
			add_filter(hash_code);
		}
		node &filter = bloom_filter->load(filter_head[hash_code]);
		BloomFilter bf(filter.p);
		node &data_block = block->load(*bf.block);
		data_block.dirty_mark = true;
		DataBlock data(data_block.p);
		char *head = data[*bf.page_cnt];
		char *tail = data[*bf.page_cnt + 1];
		// 插入bloom filter
		for (int *i = (int*)buffer.p + 1; i < (int*)buffer.head; i += 2) { // 一个int存key，一个int存value头
			bf.insert(*i);
		}
		memcpy(data[*bf.page_cnt], buffer.p, PAGE_SIZE);
//		for (int *)
		++(*bf.page_cnt);
		block->release(*bf.block);
		bloom_filter->release(filter_head[hash_code]);
		bloom_filter->release(0);
		buffer.clear();
	}


	BloomFilter load_first_filter(int hash_code);
	BloomFilter load_next_filter(const BloomFilter& cur_filter);
	DataBlock load_block(const BloomFilter& cur_filter);

	void* search(int key);
};
