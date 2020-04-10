#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include "file.h"
#include "diskmanager.h"
#include "memorymanager.h"

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
	for (int i = 0; i <= 10; ++i) {
		printf("adding item (%d, %d)\n", i, 'a' + i);
		mem.add_item(i, 'a' + i);
	}
	mem.flush_to_disk();
	BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(read_10_int_data)
{
	DiskManager disk;
	for (int i = 0; i <= 10; ++i) {
		int *ans = (int*)disk.search(i);
		BOOST_TEST(*ans == i * i - 2 * i + 1);
	}
}

BOOST_AUTO_TEST_CASE(read_write_100_int_data)
{
	BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(read_write_long_long_data)
{
	DiskManager disk;
	MemoryManager<long long> mem(disk);

	BOOST_TEST(true);
}