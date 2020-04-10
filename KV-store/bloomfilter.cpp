#include "pch.h"
#include "bloomfilter.h"


BloomFilter::BloomFilter(void *p)
{
	next = (int*)p;
	block = (int*)((char*)p + sizeof(int));
	page_cnt = (int*)((char*)p + 2 * sizeof(int));
	table = (char*)p + 3 * sizeof(int);
}


BloomFilter::~BloomFilter()
{
}

void BloomFilter::insert(const unsigned char * key_begin, const std::size_t & length)
{
	std::size_t bit_index = 0;
	std::size_t bit       = 0;
	for (std::size_t i = 0; i < HashFunction::salt_count_; ++i)
	{
		compute_indices(HashFunction::hash_ap(key_begin, length, i), bit_index, bit);
		table[bit_index / bits_per_char] |= bit_mask[bit];
	}

}

/*
template<typename T>
void BloomFilter::insert(T key) // 这里插入的时候用的临时变量应该没问题吧
{
	insert(reinterpret_cast<const unsigned char*>(&key), sizeof(T));
}*/

bool BloomFilter::contains(const unsigned char * key_begin, const std::size_t length)
{
	std::size_t bit_index = 0;
	std::size_t bit       = 0;
	
	for (std::size_t i = 0; i < HashFunction::salt_count_; ++i)
	{
		compute_indices(HashFunction::hash_ap(key_begin, length, i), bit_index, bit);
		if ((table[bit_index / bits_per_char] & bit_mask[bit]) != bit_mask[bit])
		{
			return false;
		}
	}
	return true;
}

/*
template<typename T>
bool BloomFilter::contains(T key)
{
	return contains(reinterpret_cast<const unsigned char*>(&key), sizeof(T));
}*/
