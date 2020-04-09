#pragma once

#include "header.h"
#include <vector>
#include <algorithm>

class HashFunction
{
	static std::vector<unsigned int> salt_;
	static unsigned long long        random_seed_;
	static void generate_unique_salt();

public:
	HashFunction();
	~HashFunction();
	static unsigned int              salt_count_;
	static int hash1(int key);
	static int hash2(int key);
	static unsigned int hash_ap(const unsigned char* begin, std::size_t remaining_length, int hash_num);
};

