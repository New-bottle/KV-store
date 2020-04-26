#pragma once
#include "header.h"
#include "file.h"

struct KPPair // key + ptr pair
{
	int key;
	int ptr; // offset
}; // TODO buffer can be reworked by using KPpair

template<typename value_type>
class DataPage
{
	char *p;
	/*
	[ cnt last | key ptr | key ptr | ... | ... value value ]
	*/
public:
	DataPage(void *ptr) 
	{
		p = (char*)ptr;
		// p[0] = cnt
		// p[1] = last one's offset
	}
	bool full() 
	{
		int *tmp = (int*)p;
		return 2 * sizeof(int) * (1 + tmp[0]) < tmp[1];
	}
	int lower_bound(int key)
	{
		int *head = (int*)p;
		int l = 0, r = *head - 1, mid, ans = -1;
		KPPair *ptr = (KPPair*)(p + 2 * sizeof(int));
		while (l <= r) {
			mid = (l + r) >> 1;
			if (ptr[mid].key >= key) ans = mid, r = mid - 1;
			else l = mid + 1;
		}
		if (ans == -1) ans = *head;
		return ans;
	}
	bool insert(int key, value_type value)
	{
		int *head = (int*)p;
		if ((*head + 2) * 2 * sizeof(int) >= *(head + 1) - sizeof(value_type))
			return false;
		KPPair *ptr = (KPPair*)(head + 2);
		int pos = lower_bound(key);
		for (int i = *head - 1; i >= pos; --i)
			ptr[i + 1] = ptr[i]; // move right
		ptr[pos].key = key;
		ptr[pos].ptr = *(head + 1) - sizeof(value_type);
		memcpy(p + ptr[pos].ptr, &value, sizeof(value_type));
		*(head + 1) -= sizeof(value_type);
		*head += 1;
		return true;
	}
	value_type* search(int key)
	{
		int *head = (int*)p;
		KPPair *ptr = (KPPair*)(p + 2 * sizeof(int));
		int pos = lower_bound(key);
		if (ptr[pos].key == key)
			return (value_type*)(p + ptr[pos].ptr);
		else
			return nullptr;
	}
	void split(DataPage &rhs)
	{ // put half to rhs
		char *tmp = new char[PAGE_SIZE];
		memcpy(tmp, p, PAGE_SIZE);
		this->clear();
		rhs.clear();
		int *head = (int*)tmp;
		KPPair *ptr = (KPPair*)(tmp + 2 * sizeof(int));
		int cnt = 0;
		value_type *ans;
		for (int i = (*head) / 4 * 3; i < *head; ++i) { // 1/4 goto new page
			ans = (value_type*)(tmp + ptr[i].ptr);
			rhs.insert(ptr[i].key, *ans);
		}
		for (int i = 0; i < (*head) / 4 * 3; ++i) { // 3/4 remain in old page
			ans = (value_type*)(tmp + ptr[i].ptr);
			this->insert(ptr[i].key, *ans);
		}
		delete[] tmp;
	}
	void clear()
	{
		memset(p, 0, PAGE_SIZE); // maybe time consuming
		int *head = (int *)p;
		head[0] = 0;
		head[1] = PAGE_SIZE;
	}
	int min_key()
	{
		// return the min key in the page
		return ((KPPair*)(p + 2 * sizeof(int)))->key;
	}
};

template<typename value_type>
class DataBlock
{
	File *block;
	//	int *ptr, *used;
	//	char *data;
	int header;
public:
	DataBlock(File *block, int header) :block(block), header(header) {}
	~DataBlock() {}
	bool add_item(int key, value_type value)
	{
		node &head = block->load(header);
		head.dirty_mark = true; // dirty read
		KPPair *ptr = (KPPair*)((char*)head.p + sizeof(int));
		//int &used = p[0];
		int l = 0, r = *(int*)(head.p) - 1, mid, pos = 0;
		while (l <= r) {
			mid = (l + r) >> 1;
			if (ptr[mid].key < key) pos = mid, l = mid + 1;
			else r = mid - 1;
		}
		node &blk = block->load(header + 1 + ptr[pos].ptr);
		DataPage<value_type> page(blk.p);
		if (!page.insert(key, value)) {
			int &cnt = *(int*)(head.p);
			if (cnt == BLOCK_PAGE_NUM - 1) return false; // block is full, can't split
			for (int i = cnt - 1; i > pos; ++i)
				ptr[i + 1] = ptr[i];
			++cnt;
			node &blk2 = block->load(header + 1 + cnt);
			DataPage<value_type> page2(blk2.p);
			page.split(page2);
			if (key < page2.min_key()) 
				page.insert(key, value);
			else 
				page2.insert(key, value);
			ptr[pos].key = page.min_key();
			ptr[pos + 1].ptr = cnt;
			ptr[pos + 1].key = page2.min_key();
			block->release(header + 1 + cnt);
		}
		else {
			ptr[pos].key = page.min_key();
		}
		block->release(header + 1 + ptr[pos].ptr);
		block->release(header);
		return true;
	}
	value_type* search(int key)
	{
		node &head = block->load(header);
		KPPair *ptr = (KPPair*)((char*)head.p + sizeof(int));
		int l = 0, r = *(int*)(head.p) - 1, mid, pos = 0;
		while (l <= r) {
			mid = (l + r) >> 1;
			if (ptr[mid].key <= key) pos = mid, l = mid + 1;
			else r = mid - 1;
		}
		node &blk = block->load(header + 1 + ptr[pos].ptr);
		DataPage<value_type> page(blk.p);
		value_type *ans = page.search(key);
		block->release(header + 1 + ptr[pos].ptr);
		block->release(header);
		return ans;
	}
	void clear()
	{
		node &head = block->load(header);
		head.dirty_mark = true;
		//memset(head.p, 0, sizeof(BLOCK_SIZE));
		int *ptr = (int*)head.p;
		ptr[0] = 1; // data page [0, ptr[0] - 1]
		for (int i = 1; i <= 2 * BLOCK_PAGE_NUM; ++i)
		{
			ptr[i] = 0; // key
	//		ptr[i + BLOCK_PAGE_NUM] = 0; // ptr
		}
		for (int i = 1; i < BLOCK_PAGE_NUM; ++i) {
			node &first = block->load(header + i);
			DataPage<value_type> dp(first.p);
			first.dirty_mark = true;
			dp.clear();
			block->release(header + i);
		}
		block->release(header);
	}
};

/*
[ ptr (key)  ptr (key)  ptr (key)  ptr]
[data       data       data       data]
[data       data       data       data]
 ....     
[data       data       data       data]
*/
