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
	char *p; // ָ��ʵ�ʴ洢���ݵĿռ�

public:
	Page();
	~Page();
};

