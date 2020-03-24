#include "pch.h"
#include "Memory.h"

template<typename value_type, int page_size, int hash_number>
Memory<typename value_type, page_size, hash_number>::Memory()
{
	buffer = new Buffer<value_type, page_size>[hash_number];
	// TODO
}

template<typename value_type, int page_size, int hash_number>
Memory<typename value_type, page_size, hash_number>::~Memory()
{
	delete buffer;
	delete filter_cache;
	delete block_cache;
	// TODO
}
