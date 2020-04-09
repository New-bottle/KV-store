#pragma once

#include "header.h"
#include <cstring>
template<typename value_type>
class Buffer
{
private:
	char *p;
	char *head;
	char *tail;
public:
	Buffer();
	~Buffer();
	bool add_item(int key, value_type value);
	void sort();
};

