//TODO: Add cross-platform support
#include <cstdio>
#include <map>
#include <random>
#include <utility>
#include <Windows.h>

using namespace std;

template<typename T>
class RandomHolder {
private:
	default_random_engine engine;
	map<pair<T, T>, uniform_int_distribution<T>> dists;
public:
	T get(T min, T max) {
		pair<T, T> range(min, max);
		if (dists.find(range) == dists.end()) {
			dists[range] = uniform_int_distribution<T>(range.first, range.second);
		}
		return dists[range](engine);
	}
};

int main() {
	system("color 0a");
	RandomHolder<int> holder;
	printf("--STARTING KERNEL MEMORY DUMP--\n");
	for (int i = 0; i < 5000; i++) {
		if (i % 250 == 0) {
			sectionStart:
			int pages = holder.get(0, 300);
			printf("\nSECTION 0x%x, %d VIRTUAL PAGES\n", holder.get(0, 100000), pages);
			if (!holder.get(0, 5)) {
				printf("!!PAGE CORRUPTION DETECTED. SENDING SIGSEGV TO ALL %d PROCESSES!!\n", pages);
				for (int j = 0; j < pages * 10; j++) {
					printf("%x", holder.get(0, 1000000000));
				}
				printf("\n");
				goto sectionStart;
			}
		}
		printf("%x", holder.get(0, 1000000000));
		if (!holder.get(0, 3))
			Sleep(holder.get(0, 60));
		if (!holder.get(0, 3))
			printf(" ");
	}
	printf("\n--PROCESS COMPLETE--\n");
	return 0;
}