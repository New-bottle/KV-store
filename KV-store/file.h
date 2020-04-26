#pragma once

#include "header.h"
#include <cstdio>
#include <map>

struct node
{
	void* p;
	int length;
	int count;
	bool dirty_mark;
	node(void* p=nullptr, int length=-1, int count=-1, bool dirty_mark=false) :
		p(p), length(length), count(count), dirty_mark(dirty_mark) {}
};

class File
{
	FILE *fp;
	int page_size;
	std::map<int, node> live;
	std::map<int, node> sleep;

public:
	File(FILE *fp, int page_size);
	node& load(int page_id, int length = 1);
	void release(int page_id);
	void flush(std::map<int, node>::iterator iter);
	void flush(int page_id);
	bool flush();
	~File();
};

