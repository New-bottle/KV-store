#pragma once
#include "buffer.h"
#include "header.h"
#include "bloomfilter.h"
#include "datablock.h"
#include "file.h"
#include <cstdio>
#include <io.h>

class DiskManager
{
private:
	File *block;
	File *bloom_filter;
	int block_page_cnt;
	int filter_page_cnt;
	int block_page_size;
	int filter_page_size;

	void create_block_file();
	void create_filter_file();
public:
	DiskManager();
	DiskManager(FILE *block_fp, int block_page_size, FILE *bloom_filter_fp, int bf_page_size);
	~DiskManager();
	template<typename value_type, int page_size>
	bool add_page(int hash_code, Buffer<value_type, page_size>& buffer);
};

