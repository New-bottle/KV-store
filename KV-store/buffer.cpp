#include "pch.h"
#include "Buffer.h"

template<typename value_type, int page_size>
Buffer<value_type, page_size>::Buffer()
{
	p = new char[page_size];
}

template<typename value_type, int page_size>
Buffer<value_type, page_size>::~Buffer()
{
	delete p;
}
