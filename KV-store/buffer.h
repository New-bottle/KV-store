#pragma once

#include "header.h"
#include <cstring>
template<typename value_type>
class Buffer
{
public:
	char *p;
	char *head;
	char *tail;
public:
	Buffer()
	{
		p = new char[PAGE_SIZE];
		memset(p, 0, PAGE_SIZE);
		head = p + sizeof(int); // cnt at head
		tail = p + PAGE_SIZE;
	}

	~Buffer()
	{
		delete p;
	}
	bool add_item(int key, value_type value)
	{
		// insert failed when space isn't enough
		if (head + 2 * sizeof(int) > tail - sizeof(value_type)) return false;

		memcpy(head, &key, sizeof(int));
		head += sizeof(int);
		memcpy(tail - sizeof(value_type), &value, sizeof(value_type));
		tail -= sizeof(value_type);
		int pos = tail - p;
		memcpy(head, &pos, sizeof(int));
		head += sizeof(int);
		int *cnt = (int*)p;
		++(*cnt);
		return true;
	}
	void clear() {
		memset(p, 0, PAGE_SIZE);
		head = p + sizeof(int);
		tail = p + PAGE_SIZE;
	}

	//void sort();
};

