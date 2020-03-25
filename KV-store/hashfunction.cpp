#include "pch.h"
#include "hashfunction.h"


HashFunction::HashFunction()
{
}


HashFunction::~HashFunction()
{
}

/*
 * remove the last two digits then use the remained last 4 digits
 * actually, the last 3-6th digits
 * 10034601 -> 0346
 * 12345678 -> 3456
 */
int HashFunction::hash1(int key)
{
	return key / 100 % 10000;
}

/*
 * remove the last two digits then use the remained last 2 digits
 * actually, the last 3-4th digits
 * 10034601 -> 46
 * 12345678 -> 56
 */
int HashFunction::hash2(int key)
{
	return key / 100 % 100;
}