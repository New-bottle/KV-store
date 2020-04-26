#include "datablock.h"

DataBlock::DataBlock(File *block, int header):block(block), header(header)
{
}


DataBlock::~DataBlock()
{
}

void DataBlock::clear()
{
	node &head = block->load(header);
	head.dirty_mark = true;
	int *ptr = (int*)head.p;
	
	ptr[0] = 1; // used page
	for (int i = 1; i <= 2 * BLOCK_PAGE_NUM; ++i)
	{
		ptr[i] = 0; // key
//		ptr[i + BLOCK_PAGE_NUM] = 0; // ptr
	}
	node &first = block->load(header + 1);
	first.dirty_mark = true;
	memset(first.p, 0, PAGE_SIZE);
	block->release(header);
	block->release(header + 1);
}
