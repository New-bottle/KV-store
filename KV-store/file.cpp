#include "pch.h"
#include "file.h"

// start with page_id, length's pages
node& File::load(int page_id, int length) 
{
	auto it = live.upper_bound(page_id);
	bool live_finded = true, sleep_finded = true;
	if (it == live.begin()) { // not loaded
		live_finded = false;
	}
	else {
		--it;
		if (it->first + it->second.length - 1 < page_id)
			live_finded = false;
	}

	if (live_finded) {
		if (it->first == page_id && it->second.length == length) {
			it->second.count += 1;
			return it->second;
		}
		else {
			flush(it);
		}
	}
	else {
		it = sleep.upper_bound(page_id);
		if (it == sleep.begin()) {
			sleep_finded = false;
		} else{
			--it;
			if (it->first + it->second.length - 1 < page_id)
				sleep_finded = false;
		}
		if (sleep_finded) {
			if (it->first == page_id && it->second.length == length) {
				it->second.count += 1;
				live[it->first] = it->second;
				sleep.erase(it); // maybe emplace?
				return live[page_id];
			}
			else {
				flush(it);
			}
		}
	}
	char *tmp = new char[length * page_size];
	memset(tmp, 0, length * page_size);
	long long target = (long long)page_id * page_size, now = 0;
	fseek(fp, 0, SEEK_SET);
	while (target - now >= 2LL * 1024 * 1024 * 1024) {
		fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, 1);
		now += 2LL * 1024 * 1024 * 1024 - 1;
	}
	fseek(fp, target - now, SEEK_CUR);
	fread(tmp, sizeof(char), length * page_size, fp);
	it = live.insert(std::make_pair(page_id, node(tmp, length, 1, false))).first;
	return it->second;
}

void File::release(int page_id)
{
	auto it = live.find(page_id);
	if (it == live.end()) {
		printf("page %d not found!", page_id);
		throw "page not found";
	}
	it->second.count -= 1;
	if (it->second.count == 0) {
		sleep[page_id] = it->second;
		live.erase(it);
	}
}

void File::flush(std::map<int, node>::iterator iter)
{
	if (iter->second.dirty_mark) {
		long long target = (long long)iter->first * page_size, now = 0;
		fseek(fp, 0, SEEK_SET);
		while (target - now >= 2LL * 1024 * 1024 * 1024) {
			fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, SEEK_CUR);
			now += 2LL * 1024 * 1024 * 1024 - 1;
		}
		fseek(fp, target - now, SEEK_CUR);
		fwrite(iter->second.p, sizeof(char), iter->second.length*page_size, fp);
	}
	delete[] iter->second.p;
	// erase (iter);
}

void File::flush(int page_id)
{ // TODO ? maybe need to be modified
	auto it = live.find(page_id);
	if (it != live.end()) {
		flush(it);
		live.erase(it);
	}
	else {
		it = sleep.find(page_id);
		if (it != sleep.end()) {
			flush(it);
			sleep.erase(it);
		}
	}
}

bool File::flush()
{
#ifdef DEBUG
	printf("flushing all cache\n");
#endif
	for (auto i = live.begin(); i != live.end(); ++i) {
		flush(i);
	}
	live.clear();
	for (auto i = sleep.begin(); i != sleep.end(); ++i) {
		flush(i);
	}
	sleep.clear();
	return true;
}

File::File(FILE *fp, int page_size) :fp(fp), page_size(page_size) {}

File::~File()
{
	flush();
	fclose(fp);
}
