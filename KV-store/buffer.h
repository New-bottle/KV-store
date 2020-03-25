#pragma once

template<typename value_type, int page_size>
class Buffer
{
private:
	char *p;
public:
	Buffer();
	~Buffer();
	bool add_item(int key, value_type value);
};

