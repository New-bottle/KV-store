#pragma once

#include <cstdio>
#include <vector>
#include <bitset>
#include "header.h"
#include "hashfunction.h"

static const std::size_t bits_per_char = 0x08; // 8 bits in 1 char(unsigned)
static const unsigned char bit_mask[bits_per_char] = {
	0x01, //00000001
	0x02, //00000010
	0x04, //00000100
	0x08, //00001000
	0x10, //00010000
	0x20, //00100000
	0x40, //01000000
	0x80, //10000000
};

// m:table_size, k:salt_count
class BloomFilter
{
public:
	// members
	int *next; // BloomFilter *
	int *block; // DataBlock *
	int *page_cnt; // pages in data block
	char *table;
	// static member
private:
	void compute_indices(const unsigned int& hash, std::size_t& bit_index, std::size_t& bit) const
	{
		bit_index = hash % TABLE_SIZE;
		bit       = bit_index % bits_per_char;
	}
public:
	BloomFilter(void *p);
	~BloomFilter();
	void clear();
	void insert(const unsigned char* key_begin, const std::size_t& length);
	template<typename T>
	void insert(T key)
	{
		insert(reinterpret_cast<const unsigned char*>(&key), sizeof(T));
	}
	bool contains(const unsigned char* key_begin, const std::size_t length);
	template<typename T>
	bool contains(T key)
	{
		return contains(reinterpret_cast<const unsigned char*>(&key), sizeof(T));
	}
};

