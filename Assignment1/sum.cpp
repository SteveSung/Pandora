#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include "counters.h"

int main(int argc, char *argv[]) {
	// Initialize a hardward counter
	hwCounter_t c1;
	c1.init = false;
	initTicks(c1);

	// Get the number of the user
	//int n = atoi(argv[1]);
	long long sum = 0;

	// Get time before
	uint64_t time = getTicks(c1);

	// Do some computation
	for (long long i=0; i<10000; i++)
		sum += i;

	// Get time after
	uint64_t elapsed = getTicks(c1) - time;
	printf("elapsed = %lu , sum = %lld", elapsed, sum);
}


