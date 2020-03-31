#pragma once

#include <cstdio>

struct PageHeader {
	int kv_num;
	int key_size;
	int value_size;
};

class Page
{
private:
	PageHeader header;
	FILE *fp; // 指向实际存储数据的空间

public:
	Page();
	~Page();
	PageHeader getheader();
};

/*
 * Header
 * key|key|key
 * value|value|value
 */