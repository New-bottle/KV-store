#pragma once

#include <cstdio>
#include <vector>
#include <bitset>
#include "header.h"

// m:table_size, k:salt_count
class BloomFilter
{
private:
	// members
	int salt_count;
	int table_size;
	char *table;
	int next; // BloomFilter *
	int block; // DataBlock *
	// static member

	typedef unsigned int bloom_type;
	typedef unsigned char cell_type;

	std::vector<bloom_type> salt_;
	unsigned long int random_seed_;
	bloom_type hash_ap(const unsigned char* begin, std::size_t remaining_length, bloom_type hash) const;
	
public:
	BloomFilter();
	~BloomFilter();
	BloomFilter(int table_size, char *table, int next, int block);
	void generate_unique_salt(); // init when system start
};

