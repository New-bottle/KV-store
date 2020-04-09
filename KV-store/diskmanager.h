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
	int *block_page_cnt;
	int *filter_page_cnt;
	int *block_page_size;
	int *filter_page_size;

	void create_block_file();
	void create_filter_file();

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
	void add_page(int hash_code, Buffer<value_type>& buffer);

	BloomFilter load_first_filter(int hash_code);
	BloomFilter load_next_filter(const BloomFilter& cur_filter);
	DataBlock load_block(const BloomFilter& cur_filter);

	template<typename value_type>
	value_type search(int key);
};
