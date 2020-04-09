#pragma once
#include "header.h"
class DataBlock
{
	void *data;
public:
	char* operator [] (int page_num) const;
	DataBlock(void *ptr);
	~DataBlock();
};

