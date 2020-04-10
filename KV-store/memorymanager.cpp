#include "pch.h"
#include "MemoryManager.h"

template<typename value_type>
MemoryManager<value_type>::MemoryManager()
{
	buffer = nullptr;
}

template<typename value_type>
MemoryManager<value_type>::~MemoryManager()
{
	if (buffer != nullptr) delete buffer;
}

template<typename value_type>
bool MemoryManager<value_type>::init_buffer()
{
	try {
		buffer = new Buffer<value_type>[HASH_NUMBER];
	}
	catch (...){
		return false;
	}
	return true;
}

template<typename value_type>
bool MemoryManager<value_type>::add_item(int key, value_type value)
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
		flush_to_disk(buffer[hash_code], hash_code); // maybe here will be false?
		buffer[hash_code].add_item(key, value);
		return true;
	}
}

template<typename value_type>
void MemoryManager<value_type>::flush_to_disk(int hash_code, DiskManager& disk_manager)
{
	disk_manager.add_page(hash_code, buffer[hash_code]);
}

template<typename value_type>
void MemoryManager<value_type>::flush_to_disk(DiskManager & disk_manager)
{
	for (int i = 0; i < HASH_NUMBER; ++i)
		flush_to_disk(i, disk_manager);
}

/*
template<typename value_type>
bool MemoryManager<typename value_type>::flush_to_disk(const DiskManager& disk_manager, Buffer<value_type, page_size>& buffer, int hash_code) 
{
	disk_manager.add_page(hash_code, buffer);
}
*/

