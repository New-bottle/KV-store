#pragma once

#include "page.h"

class DataBlock
{
private:
	int block_size;
	Page *pages; // 指向存储数据的pages
public:
	DataBlock();
	~DataBlock();
};

