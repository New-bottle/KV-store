#pragma once
#include "buffer.h"
#include "diskmanager.h"
#include "hashfunction.h"

template<typename value_type>
class MemoryManager
{
private:
	Buffer<value_type> *buffer;
public:
	MemoryManager();
	~MemoryManager();
	bool init_buffer();
	bool add_item(int key, value_type value);
	void flush_to_disk(int hash_code, DiskManager& disk_manager);
	void flush_to_disk(DiskManager& disk_manager);
	//bool flush_to_disk(const DiskManager& disk_manager, Buffer<value_type>& buffer, int hash_code);
};

