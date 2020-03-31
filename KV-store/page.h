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
	FILE *fp; // ָ��ʵ�ʴ洢���ݵĿռ�

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