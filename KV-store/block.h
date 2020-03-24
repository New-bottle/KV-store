#pragma once

template<int block_size>
class Block
{
private:
	Page *pages; // 指向存储数据的pages
public:
	Block();
	~Block();
};

