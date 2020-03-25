#pragma once
#include "buffer.h"
#include "header.h"
#include "bloomfilter.h"
#include "block.h"
#include <cstdio>

class DiskManager
{
private:
	FILE *block_fp;
	FILE *bloom_filter_fp;

public:
	DiskManager();
	~DiskManager();
	template<typename value_type, int page_size>
	bool add_page(int hash_code, Buffer<value_type, page_size>& buffer);
};

