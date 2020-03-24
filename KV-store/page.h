#pragma once

template<int page_size>
class Page
{
private:
	struct Header {
		int kv_num;
		int key_size;
		int value_size;
	} header;
	char *p; // 指向实际存储数据的空间

public:
	Page();
	~Page();
};

