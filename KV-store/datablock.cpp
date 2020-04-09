#include "datablock.h"



char * DataBlock::operator[](int page_num) const
{
	return (char*)data + page_num * PAGE_SIZE;
}

DataBlock::DataBlock(void *ptr):data(ptr)
{
}


DataBlock::~DataBlock()
{
}
