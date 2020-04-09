#include "pch.h"
#include "Buffer.h"

template<typename value_type>
Buffer<value_type>::Buffer()
{
	p = new char[PAGE_SIZE];
	memset(p, 0, PAGE_SIZE);
	head = p + sizeof(int); // cnt at head
	tail = p + PAGE_SIZE;
}

template<typename value_type>
Buffer<value_type>::~Buffer()
{
	delete p;
}

template<typename value_type>
bool Buffer<value_type>::add_item(int key, value_type value)
{
	// insert failed when space isn't enough
	if (head + 2 * sizeof(int) > tail - sizeof(value_type)) return false;

	memcpy(head, &key, sizeof(int));
	head += sizeof(int);
	memmcpy(tail - sizeof(value_type), &value, sizeof(value_type));
	tail -= sizeof(value_type);
	int pos = tail - p;
	memcpy(head, &pos, sizeof(int));
	head += sizeof(int);
	int *cnt = p;
	++(*cnt);
	return true;
}

template<typename value_type>
void Buffer<value_type>::sort()
{
	// TODO
}
