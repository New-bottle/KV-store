#pragma once

#define DEBUG

#ifndef PAGE_SIZE
#define PAGE_SIZE (16 * 1024) // 8 * 1024
//#define PAGE_SIZE 512
#endif

#ifndef HASH_NUMBER
#define HASH_NUMBER 1024
#endif

#ifndef TABLE_SIZE
#define TABLE_SIZE 65536
//#define TABLE_SIZE 512
#endif

#ifndef SALT_COUNT
#define SALT_COUNT 10
#endif

#ifndef BLOCK_FILE_NAME
#define BLOCK_FILE_NAME "block.dat"
#endif

#ifndef FILTER_FILE_NAME
#define FILTER_FILE_NAME "filter.dat"
#endif

#ifndef BLOCK_SIZE
#define BLOCK_SIZE (512 * 1024)
#endif

#ifndef BLOCK_PAGE_SIZE
#define BLOCK_PAGE_SIZE (8 * 1024)
#endif

#ifndef BLOCK_PAGE_NUM
#define BLOCK_PAGE_NUM (BLOCK_SIZE / BLOCK_PAGE_SIZE)
#endif
// warning: there should be BLOCK_PAGE_SIZE > BLOCK_PAGE_NUM * 2 * sizeof(int)

#ifndef FILTER_PAGE_SIZE
#define FILTER_PAGE_SIZE (32 * 1024)
//#define FILTER_PAGE_SIZE (1 * 1024) // 1000 int
// (hash_number + 2) * 4 <= filter_page_size
#endif

