#include "pch.h"
#include "filterblock.h"


FilterBlock::FilterBlock()
{
}


FilterBlock::~FilterBlock()
{
}

BloomFilter FilterBlock::read_data(int table_size) // table_size by byte
{
	char *tmp = new char[table_size]; // 不用delete吧应该 TODO
	fread(tmp, sizeof(char), table_size, fp);
	int ptr[2];
	fread(ptr, sizeof(int), 2, fp);
	return BloomFilter(table_size, tmp, ptr[0], ptr[1]);
}