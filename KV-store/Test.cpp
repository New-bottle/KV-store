#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include "file.h"
#include "diskmanager.h"

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
	BOOST_CHECK(disk.block_page_size == BLOCK_PAGE_SIZE);
	BOOST_TEST(disk.block_page_cnt == 0);
	//BOOST_REQUIRE(true);
}