//TODO: Add cross-platform support
#include <chrono>
#include <map>
#include <random>
#include <cstdio>
#include <utility>
#ifdef _WIN32
#include <Windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <time.h>
#endif

using namespace std;

template<typename T>
class RandomHolder {
private:
	default_random_engine engine;
	map<pair<T, T>, uniform_int_distribution<T>> dists;
public:
	RandomHolder() : engine(std::chrono::system_clock::now().time_since_epoch().count()) {}
	T get(T min, T max) {
		pair<T, T> range(min, max);
		if (dists.find(range) == dists.end()) {
			dists[range] = uniform_int_distribution<T>(range.first, range.second);
		}
		return dists[range](engine);
	}
};

void csleep(int millis) {
#ifdef _WIN32
	Sleep(millis);
#elif defined(__unix__) || defined(__APPLE__)
	struct timespec spec;
	spec.tv_sec = millis / 1000;
	spec.tv_nsec = (millis % 1000) * 1000000;
	nanosleep(spec, NULL);
#endif
}

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