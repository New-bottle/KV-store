#include "pch.h"
#include "MemoryManager.h"

template<typename value_type, int page_size, int hash_number>
MemoryManager<typename value_type, page_size, hash_number>::MemoryManager()
{
	buffer = filter_cache = block_cache = nullptr;
}

template<typename value_type, int page_size, int hash_number>
MemoryManager<typename value_type, page_size, hash_number>::~MemoryManager()
{
	if (buffer != nullptr) delete buffer;
	if (filter_cache != nullptr) delete filter_cache;
	if (block_cache != nullptr) delete block_cache;
}

template<typename value_type, int page_size, int hash_number>
bool MemoryManager<typename value_type, page_size, hash_number>::init_buffer()
{
	try {
		buffer = new Buffer<value_type, page_size>[hash_number];
	}
	catch {
		return false;
	}
	return true;
}

template<typename value_type, int page_size, int hash_number>
bool MemoryManager<typename value_type, page_size, hash_number>::add_item(int key, value_type value)
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

template<typename value_type, int page_size, int hash_number>
bool MemoryManager<typename value_type, page_size, hash_number>::flush_to_disk(const DiskManager& disk_manager, Buffer<value_type, page_size>& buffer, int hash_code) 
{
	disk_manager.add_page(hash_code, buffer);
}

