#include "pch.h"
#include "file.h"

node& File::load(int page_id)
{
	auto it = live.find(page_id);
	if (it == live.end()) {
		it = sleep.find(page_id);
		if (it == sleep.end()) {
			char *tmp = new char[page_size];
			memset(tmp, 0, sizeof(page_size));
			long long target = (long long)page_id * page_size, now = 0;
			fseek(fp, 0, SEEK_SET);
			while (target - now >= 2LL * 1024 * 1024 * 1024) {
				fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, 1);
				now += 2LL * 1024 * 1024 * 1024 - 1;
			}
			fseek(fp, target - now, SEEK_CUR);
			//fseek(fp, page_id * page_size, 0); // TODO page_id * page_size > long?
			fread(tmp, sizeof(char), page_size, fp);
			// live[page_id] = node(tmp, page_size, 1, false);
			it = live.insert(std::make_pair(page_id, node(tmp, page_size, 1, false))).first;
		}
		else {
			it->second.count += 1;
			live[it->first] = it->second;
			//live.insert(std::pair<int,node>(it->first,it->second));
			sleep.erase(it);
			it = live.find(page_id);
		}
	}
	else {
		it->second.count += 1;
	}
	return it->second;
}

void File::release(int page_id)
{
	if (live.find(page_id) == live.end()) {
		printf("page %d not found!", page_id);
		throw "page not found";
	}
	live[page_id].count -= 1;
	if (live[page_id].count == 0) {
		sleep[page_id] = live[page_id];
		live.erase(page_id);
	}
}

void File::flush(int page_id)
{
	if (live.find(page_id) != live.end()) {
		if (live[page_id].dirty_mark) {
			long long target = (long long)page_id * page_size, now = 0;
			fseek(fp, 0, SEEK_SET);
			while (target - now >= 2LL * 1024 * 1024 * 1024) {
				fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, 1);
				now += 2LL * 1024 * 1024 * 1024 - 1;
			}
			fseek(fp, target - now, SEEK_CUR);
			fwrite(live[page_id].p, sizeof(char), page_size, fp);
		}
		delete[] live[page_id].p;
		live.erase(page_id);
	}
	else if (sleep.find(page_id) != sleep.end()) {
		if (sleep[page_id].dirty_mark) {
			long long target = (long long)page_id * page_size, now = 0;
			fseek(fp, 0, SEEK_SET);
			while (target - now >= 2LL * 1024 * 1024 * 1024) {
				fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, 1);
				now += 2LL * 1024 * 1024 * 1024 - 1;
			}
			fseek(fp, target - now, SEEK_CUR);
			fwrite(sleep[page_id].p, sizeof(char), page_size, fp);
		}
		delete[] sleep[page_id].p;
		sleep.erase(page_id);
	}
}

bool File::flush()
{
#ifdef DEBUG
	printf("flushing all cache\n");
#endif
	for (auto i = live.begin(); i != live.end(); ++i) {
		if (i->second.dirty_mark) {
			long long target = (long long)i->first * page_size, now = 0;
			fseek(fp, 0, 0);
			while (target - now >= 2LL * 1024 * 1024 * 1024) {
				fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, 1);
				now += 2LL * 1024 * 1024 * 1024 - 1;
			}
			fseek(fp, target - now, 1);
			fwrite(i->second.p, sizeof(char), page_size, fp);
		}
		delete[] i->second.p;
	}
	live.clear();
	for (auto i = sleep.begin(); i != sleep.end(); ++i) {
		if (i->second.dirty_mark) {
			long long target = (long long)i->first * page_size, now = 0;
			fseek(fp, 0, 0);
			while (target - now >= 2LL * 1024 * 1024 * 1024) {
				fseek(fp, 2LL * 1024 * 1024 * 1024 - 1, 1);
				now += 2LL * 1024 * 1024 * 1024 - 1;
			}
			fseek(fp, target - now, 1);
			fwrite(i->second.p, sizeof(char), page_size, fp);
		}
		delete[] i->second.p;
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
