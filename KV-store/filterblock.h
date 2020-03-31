#pragma once

#include <cstdio>
#include "bloomfilter.h"
#include "header.h"

class FilterBlock
{
private:
	FILE *fp;

public:
	FilterBlock();
	~FilterBlock();
	BloomFilter read_data(int table_size);
};

