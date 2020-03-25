#include "pch.h"
#include "diskmanager.h"


DiskManager::DiskManager()
{
	// open files
}


DiskManager::~DiskManager()
{
	// close files
}

template<typename value_type, int page_size>
bool DiskManager::add_page(int hash_code, Buffer<value_type, page_size>& buffer)
{

}