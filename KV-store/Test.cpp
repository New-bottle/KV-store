#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include "file.h"
#include "diskmanager.h"
#include "memorymanager.h"

/*
BOOST_AUTO_TEST_CASE(myTestCase)
{
  BOOST_TEST((int)(1.6) == 1);
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(fileTestCase)
{
	FILE *fp = fopen("filetest.dat", "wb+");
	File test_file(fp, 512);
	node &header = test_file.load(0);
	int *tmp = (int*)(header.p);
	for (int i = 0; i < 10; ++i)
		tmp[i] = i;
	header.dirty_mark = true;
	test_file.release(0);
	test_file.flush(0);

	node &hd = test_file.load(0);
	tmp = (int*)(hd.p);
	bool flag = true;
	for (int i = 0; i < 10; ++i) {
		printf("%d\n", i);
		if (tmp[i] != i)
			flag = false;
	}
	test_file.flush(0);
	BOOST_REQUIRE(flag == true);
}

BOOST_AUTO_TEST_CASE(diskManager)
{
	DiskManager disk;
	BOOST_CHECK(*disk.block_page_size == BLOCK_PAGE_SIZE);
	BOOST_TEST(*disk.block_page_cnt == 0);
	//BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE(read_write_10_int_data)
{
	DiskManager disk;
	MemoryManager<int> mem(disk);
	mem.init_buffer();
	for (int i = 1; i <= 10; ++i) {
		//printf("adding item (%d, %d)\n", i, i * i + 2 * i - 1);
		mem.add_item(i, i * i + 2 * i - 1);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
	for (int i = 1; i <= 10; ++i) {
		int *ans = (int*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
}

BOOST_AUTO_TEST_CASE(read_write_1000_int_data)
{
	DiskManager disk;
	MemoryManager<int> mem(disk);
	mem.init_buffer();
	for (int i = 0; i <= 1000; ++i) {
		//printf("adding item (%d, %d)\n", i, i * i + 2 * i - 1);
		mem.add_item(i, i * i + 2 * i - 1);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
	for (int i = 0; i <= 1000; ++i) {
		int *ans = (int*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}

BOOST_AUTO_TEST_CASE(read_write_100000_int_data)
{
	DiskManager disk;
	MemoryManager<int> mem(disk);
	mem.init_buffer();
	for (int i = 0; i <= 100000; ++i) {
		//printf("adding item (%d, %d)\n", i, i * i + 2 * i - 1);
		mem.add_item(i, i * i + 2 * i - 1);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
	for (int i = 0; i <= 100000; ++i) {
		int *ans = (int*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}

BOOST_AUTO_TEST_CASE(read_100000_int_data)
{
	DiskManager disk;
	BOOST_TEST(true);
	for (int i = 0; i <= 100000; ++i) {
		int *ans = (int*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}

BOOST_AUTO_TEST_CASE(read_write_100000_long_long_data)
{
	DiskManager disk;
	MemoryManager<long long> mem(disk);
	mem.init_buffer();
	for (long long i = 0; i <= 100000; ++i) {
		mem.add_item(i, i * i + 2 * i - 1);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
	for (long long i = 0; i <= 100000; ++i) {
		long long *ans = (long long*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
	BOOST_TEST(true);
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}

BOOST_AUTO_TEST_CASE(read_write_200000_long_long_data)
{
	DiskManager disk;
	MemoryManager<long long> mem(disk);
	mem.init_buffer();
	for (long long i = 0; i <= 200000; ++i) {
		mem.add_item(i, i * i + 2 * i - 1);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
	for (long long i = 0; i <= 200000; ++i) {
		long long *ans = (long long*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
	BOOST_TEST(true);
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}
*/


BOOST_AUTO_TEST_CASE(write_1000000_long_long_data)
{
	DiskManager disk;
	MemoryManager<long long> mem(disk);
	mem.init_buffer();
	for (long long i = 0; i <= 10000000; ++i) {
		mem.add_item(i, i * i + 2 * i - 1);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}

BOOST_AUTO_TEST_CASE(read_1000000_long_long_data)
{
	DiskManager disk;
	for (long long i = 0; i <= 10000000; ++i) {
		long long *ans = (long long*)disk.search(i);
		BOOST_TEST(*ans == i * i + 2 * i - 1);
	}
#ifdef DEBUG
	printf("filter_cnt: %d   block_cnt: %d\n", disk.filter_cnt, disk.block_cnt);
#endif
}
