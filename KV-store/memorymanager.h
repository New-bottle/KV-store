#pragma once
#include "buffer.h"
#include "diskmanager.h"
#include "hashfunction.h"

template<typename value_type, int page_size, int hash_number>
class MemoryManager
{
private:
	Buffer<value_type, page_size> *buffer;
public:
	MemoryManager();
	~MemoryManager();
	bool init_buffer();
	bool add_item(int key, value_type value);
	bool flush_to_disk(const DiskManager& disk_manager, Buffer<value_type, page_size>& buffer, int hash_code);
};

