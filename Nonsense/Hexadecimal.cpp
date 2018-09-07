#include <algorithm>
#include <cctype>
#include <chrono>
#include <map>
#include <cmath>
#include <random>
#include <cstdio>
#include <string>
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
		if(dists.find(range) == dists.end()) {
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
	nanosleep(&spec, NULL);
#endif
}

//Made because of GCC rejecting isdigit itself as a UnaryPredicate
bool isDigitPred(char c) {
	return isdigit(c);
}

bool isValidNum(string s) {
	return !s.empty() && all_of(s.begin(), s.end(), isDigitPred);
}

int main(int argc, char** argv) {
	//Read variables from command-line arguments
	//sleepChance: Actual chance of sleeping per number is 1 / (sleepChance + 1)
	int sleepChance, iterations;
	const int nOfRead = 2;
	int *toRead[nOfRead] = {&sleepChance, &iterations};
	int defaultValues[nOfRead] = {3, 5000};
	for(int i = 1;i <= nOfRead;i++) {
		if(i < argc && isValidNum(argv[i]))
			*toRead[i - 1] = strtol(argv[i], NULL, 10);
		else
			*toRead[i - 1] = defaultValues[i - 1];
	}
#ifdef _WIN32
	system("color 0a");
#elif defined(__unix__) || defined(__APPLE__)
	printf("\033[40;1;32m");
#endif
	setvbuf(stdout, NULL, _IONBF, 64);
	RandomHolder<int> holder;
	printf("--STARTING KERNEL MEMORY DUMP WITH ARGUMENTS %d/%d--\n", sleepChance, iterations);
	for(int i = 0; i < iterations; i++) {
		if(i % 250 == 0) {
		sectionStart:
			int pages = holder.get(0, 300);
			printf("\nSECTION 0x%x, %d VIRTUAL PAGES\n", holder.get(0, 100000), pages);
			if(!holder.get(0, 5)) {
				printf("!!PAGE CORRUPTION DETECTED. SENDING SIGSEGV TO ALL %d PROCESSES!!\n", pages);
				for (int j = 0; j < pages * 10; j++) {
					printf("%x", holder.get(0, 1000000000));
				}
				printf("\n");
				goto sectionStart;
			}
		}
		printf("%x", holder.get(0, 1000000000));
		if(!holder.get(0, sleepChance))
			csleep(holder.get(0, 60));
		if(!holder.get(0, 3))
			printf(" ");
	}
	printf("\n--PROCESS COMPLETE--\n");
	getchar();
	return 0;
}
