#pragma once

#include "page.h"

class DataBlock
{
private:
	int block_size;
	Page *pages; // ָ��洢���ݵ�pages
public:
	DataBlock();
	~DataBlock();
};

