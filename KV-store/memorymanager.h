#pragma once
#include "buffer.h"
#include "diskmanager.h"
#include "hashfunction.h"

template<typename value_type>
class MemoryManager
{
private:
	Buffer<value_type> *buffer;
	DiskManager& disk_manager;

public:
	MemoryManager(DiskManager& disk):disk_manager(disk)
	{
		buffer = nullptr;
	}

	~MemoryManager()
	{
#ifdef DEBUG
		printf("destructing MemoryManager\n");
#endif
		if (buffer != nullptr) {
			delete[] buffer;
		}
	}

	bool init_buffer()
	{
		try {
			buffer = new Buffer<value_type>[HASH_NUMBER];
		}
		catch (...){
			return false;
		}
		return true;
	}


	bool add_item(int key, value_type value)
	{
		if (buffer == nullptr) {
			init_buffer();
		}
		// if buffer not initialized, init
		int hash_code = HashFunction::hash1(key);
		// hash_code needs to be changed here
		if (buffer[hash_code].add_item(key, value)) {
			return true;
		}
		else {
			flush_to_disk(hash_code);
			buffer[hash_code].add_item(key, value);
			return true;
		}
		return false;
	}

	void flush_to_disk(int hash_code)
	{
		buffer[hash_code].sort();
		disk_manager.add_page(hash_code, buffer[hash_code]);
	}

	void flush_to_disk()
	{
		for (int i = 0; i < HASH_NUMBER; ++i)
			flush_to_disk(i);
	}
	//bool flush_to_disk(const DiskManager& disk_manager, Buffer<value_type>& buffer, int hash_code);
};

