#include <algorithm>
#include <cctype>
#include <chrono>
#include <map>
#include <cmath>
#include <random>
#include <cstdio>
#include <string>
#include <utility>
#include <stdexcept>
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

int main(int argc, char** argv) {
	//Read variables from command-line arguments
	if(argc >= 2 && string(argv[1]) == "-h") {
		printf("Usage: hexadecimal [sleepChance] [iterations]\n"
				"Will print (iterations) random hexadecimal numbers along with some text.\n"
				"Will never stop until interrupted if (iterations) < 0.\n"
				"After printing each number, has a (1 / sleepChance) chance to sleep for [0-60] milliseconds.\n");
		return 0;
	}
	int sleepChance, iterations;
	bool noStop = false;
	const int nOfRead = 2;
	int *toRead[nOfRead] = {&sleepChance, &iterations};
	int defaultValues[nOfRead] = {4, 5000};
	for(int i = 1;i <= nOfRead;i++) {
		if(i < argc) try {
			*toRead[i - 1] = stoi(argv[i], NULL, 10);
		}
		catch(exception e) { //Not numeric or out of range
			*toRead[i - 1] = defaultValues[i - 1];
			printf("Argument %s was invalid.\n", argv[i]);
		}
		else *toRead[i - 1] = defaultValues[i - 1];
	}
	if(iterations < 0) noStop = true;

#ifdef _WIN32
	system("color 0a");
#elif defined(__unix__) || defined(__APPLE__)
	printf("\033[40;1;32m");
#endif
	setvbuf(stdout, NULL, _IONBF, 64);
	RandomHolder<int> holder;
	printf("--STARTING KERNEL MEMORY DUMP WITH ARGUMENTS %d/%d--\n", sleepChance, iterations);
	sleepChance--; //After this, actual chance of sleeping per number is 1 / (sleepChance + 1)

	for(int i = 0;noStop || i < iterations;i++) {
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
