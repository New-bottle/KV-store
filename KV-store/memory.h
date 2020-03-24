#pragma once
#include  "Buffer.h"
template<typename value_type, int page_size, int hash_number>
class Memory
{
private:
	Buffer<value_type, page_size> *buffer;
	FilterCache<> *filter_cache;
	BlockCache<> *block_cache;
public:
	Memory();
	~Memory();
};

