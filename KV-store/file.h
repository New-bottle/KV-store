#pragma once

#include <cstdio>
#include <map>

struct node
{
	void* p;
	int length;
	int count;
	bool dirty_mark;
	node(void* p, int length, int count, bool dirty_mark) :
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
	node& load(int page_id);
	void release(int page_id);
	void flush(int page_id);
	bool flush();
	~File();
};

