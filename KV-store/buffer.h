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
		head = p + sizeof(int); // head[0] indicates the number of element in buffer (cnt)
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
	void clear() 
	{
		memset(p, 0, PAGE_SIZE);
		head = p + sizeof(int); // head[0] indicates the number of element in buffer
		tail = p + PAGE_SIZE;
	}

	void swap(int &a, int &b) 
	{
		int tmp = b;
		b = a;
		a = tmp;
	}
	void sort(int *l, int *r)
	{
		if (l >= r) return;
		int *i = l, *j = r;
		swap(l[0], l[(r - l) >> 2 << 1]); // set the mid as the key
		swap(l[1], l[(r - l) >> 2 << 1 | 1]);
		int key = l[0];
		// [key, ptr, key, ptr, key, ptr, ..., key, ptr]
		//   l                                          r
		//                           r-l/2
		//                     r-l/2/2*2
		while (i < j) {
			while (i < j && j > l && *j >= key) j -= 2;
			while (i < j && i < r && *i <= key) i += 2;
			swap(i[0], j[0]);
			swap(i[1], j[1]);
		}
		swap(j[0], l[0]);
		swap(j[1], l[1]);
		sort(l, i - 2);
		sort(i + 2, r);
	}
	void sort() 
	{
		sort((int*)p + 1, (int*)head - 2);
	}
};

