#include "pch.h"
#include "hashfunction.h"

unsigned long long HashFunction::random_seed_ = 0LL;
unsigned int HashFunction::salt_count_ = SALT_COUNT;
std::vector<unsigned int> HashFunction::salt_;

void HashFunction::generate_unique_salt()
{
	// referenced from C++ Bloom Filter Library
	/*
	Note:
	A distinct hash function need not be implementation-wise
	distinct. In the current implementation "seeding" a common
	hash function with different values seems to be adequate.
  */
	const unsigned int predef_salt_count = 128;

	static const unsigned int predef_salt[predef_salt_count] =
	{
	   0xAAAAAAAA, 0x55555555, 0x33333333, 0xCCCCCCCC,
	   0x66666666, 0x99999999, 0xB5B5B5B5, 0x4B4B4B4B,
	   0xAA55AA55, 0x55335533, 0x33CC33CC, 0xCC66CC66,
	   0x66996699, 0x99B599B5, 0xB54BB54B, 0x4BAA4BAA,
	   0xAA33AA33, 0x55CC55CC, 0x33663366, 0xCC99CC99,
	   0x66B566B5, 0x994B994B, 0xB5AAB5AA, 0xAAAAAA33,
	   0x555555CC, 0x33333366, 0xCCCCCC99, 0x666666B5,
	   0x9999994B, 0xB5B5B5AA, 0xFFFFFFFF, 0xFFFF0000,
	   0xB823D5EB, 0xC1191CDF, 0xF623AEB3, 0xDB58499F,
	   0xC8D42E70, 0xB173F616, 0xA91A5967, 0xDA427D63,
	   0xB1E8A2EA, 0xF6C0D155, 0x4909FEA3, 0xA68CC6A7,
	   0xC395E782, 0xA26057EB, 0x0CD5DA28, 0x467C5492,
	   0xF15E6982, 0x61C6FAD3, 0x9615E352, 0x6E9E355A,
	   0x689B563E, 0x0C9831A8, 0x6753C18B, 0xA622689B,
	   0x8CA63C47, 0x42CC2884, 0x8E89919B, 0x6EDBD7D3,
	   0x15B6796C, 0x1D6FDFE4, 0x63FF9092, 0xE7401432,
	   0xEFFE9412, 0xAEAEDF79, 0x9F245A31, 0x83C136FC,
	   0xC3DA4A8C, 0xA5112C8C, 0x5271F491, 0x9A948DAB,
	   0xCEE59A8D, 0xB5F525AB, 0x59D13217, 0x24E7C331,
	   0x697C2103, 0x84B0A460, 0x86156DA9, 0xAEF2AC68,
	   0x23243DA5, 0x3F649643, 0x5FA495A8, 0x67710DF8,
	   0x9A6C499E, 0xDCFB0227, 0x46A43433, 0x1832B07A,
	   0xC46AFF3C, 0xB9C8FFF0, 0xC9500467, 0x34431BDF,
	   0xB652432B, 0xE367F12B, 0x427F4C1B, 0x224C006E,
	   0x2E7E5A89, 0x96F99AA5, 0x0BEB452A, 0x2FD87C39,
	   0x74B2E1FB, 0x222EFD24, 0xF357F60C, 0x440FCB1E,
	   0x8BBE030F, 0x6704DC29, 0x1144D12F, 0x948B1355,
	   0x6D8FD7E9, 0x1C11A014, 0xADD1592F, 0xFB3C712E,
	   0xFC77642F, 0xF9C4CE8C, 0x31312FB9, 0x08B0DD79,
	   0x318FA6E7, 0xC040D23D, 0xC0589AA7, 0x0CA5C075,
	   0xF874B172, 0x0CF914D5, 0x784D3280, 0x4E8CFEBC,
	   0xC569F575, 0xCDB2A091, 0x2CC016B4, 0x5C5F4421
	};

	if (salt_count_ <= predef_salt_count)
	{
		std::copy(predef_salt,
			predef_salt + salt_count_,
			std::back_inserter(salt_));

		for (std::size_t i = 0; i < salt_.size(); ++i)
		{
			/*
			   Note:
			   This is done to integrate the user defined random seed,
			   so as to allow for the generation of unique bloom filter
			   instances.
			*/
			salt_[i] = salt_[i] * salt_[(i + 3) % salt_.size()] + static_cast<unsigned int>(random_seed_);
		}
	}
	else
	{
		std::copy(predef_salt, predef_salt + predef_salt_count, std::back_inserter(salt_));

		srand(static_cast<unsigned int>(random_seed_));

		while (salt_.size() < salt_count_)
		{
			unsigned int current_salt = static_cast<unsigned int>(rand()) * static_cast<unsigned int>(rand());

			if (0 == current_salt)
				continue;

			if (salt_.end() == std::find(salt_.begin(), salt_.end(), current_salt))
			{
				salt_.push_back(current_salt);
			}
		}
	}
}

HashFunction::HashFunction()
{
	generate_unique_salt();
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
int HashFunction::hash2(int key)
{
	return key / 100 % 10000;
}

/*
 * remove the last two digits then use the remained last 2 digits
 * actually, the last 3-4th digits
 * 10034601 -> 46
 * 12345678 -> 56
 */
int HashFunction::hash1(int key)
{
	return key / 100 % 100;
}

unsigned int HashFunction::hash_ap(const unsigned char* begin, std::size_t remaining_length, int hash_num)
{
	unsigned int hash = salt_[hash_num];
	// referenced from C++ Bloom Filter Library
	const unsigned char* itr = begin;
	unsigned int loop = 0;

	while (remaining_length >= 8)
	{
		const unsigned int& i1 = *(reinterpret_cast<const unsigned int*>(itr)); itr += sizeof(unsigned int);
		const unsigned int& i2 = *(reinterpret_cast<const unsigned int*>(itr)); itr += sizeof(unsigned int);

		hash ^= (hash << 7) ^ i1 * (hash >> 3) ^
			(~((hash << 11) + (i2 ^ (hash >> 5))));

		remaining_length -= 8;
	}

	if (remaining_length)
	{
		if (remaining_length >= 4)
		{
			const unsigned int& i = *(reinterpret_cast<const unsigned int*>(itr));

			if (loop & 0x01)
				hash ^= (hash << 7) ^ i * (hash >> 3);
			else
				hash ^= (~((hash << 11) + (i ^ (hash >> 5))));

			++loop;

			remaining_length -= 4;

			itr += sizeof(unsigned int);
		}

		if (remaining_length >= 2)
		{
			const unsigned short& i = *(reinterpret_cast<const unsigned short*>(itr));

			if (loop & 0x01)
				hash ^= (hash << 7) ^ i * (hash >> 3);
			else
				hash ^= (~((hash << 11) + (i ^ (hash >> 5))));

			++loop;

			remaining_length -= 2;

			itr += sizeof(unsigned short);
		}

		if (remaining_length)
		{
			hash += ((*itr) ^ (hash * 0xA5A5A5A5)) + loop;
		}
	}

	return hash;
}

HashFunction hash_functions;