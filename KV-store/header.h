#pragma once

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 524288 // 512 * 1024
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE 8192 // 8 * 1024
#endif

#ifndef HASH_NUMBER
#define HASH_NUMBER 100
#endif

#ifndef TABLE_SIZE
#define TABLE_SIZE 512
#endif

#ifndef SALT_COUNT
#define SALT_COUNT 5
#endif

#ifndef BLOCK_FILE_NAME
#define BLOCK_FILE_NAME "block.dat"
#endif

#ifndef FILTER_FILE_NAME
#define FILTER_FILE_NAME "filter.dat"
#endif

#ifndef BLOCK_PAGE_SIZE
#define BLOCK_PAGE_SIZE (512*1024)
#endif

#ifndef FILTER_PAGE_SIZE
#define FILTER_PAGE_SIZE (8192)
#endif