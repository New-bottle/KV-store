#include "pch.h"
#include "page.h"

template<int page_size>
Page<page_size>::Page()
{
}

template<int page_size>
Page<page_size>::~Page()
{
}

template<int page_size>
PageHeader Page<page_size>::getheader()
{
	Header ans[1], *tmp = ans;
	fread(tmp, sizeof(Header), 1, fp);
	return ans[0];
}