#pragma once

template<int block_size>
class Block
{
private:
	Page *pages; // ָ��洢���ݵ�pages
public:
	Block();
	~Block();
};

