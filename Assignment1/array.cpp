#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <math.h>
#include "counters.h"

#define N 2097152

int main(int argc, char *argv[]) {

// Initialize a hardward counter
	hwCounter_t c1;
	c1.init = false;
	initTicks(c1);

// Do some computation
	srand(time(NULL));
	int array[N];
    int cnt;
	int i;
	uint64_t twototwenty = (uint64_t)pow(2,20);

	//printf("%d",N);
	
	for(int j=0;j<N;j++)
	{
		array[j] = rand()%N;
	}

	
	i = 0;


// Get time befor
	uint64_t before_time = getTicks(c1);

	for(cnt=0; cnt < twototwenty; cnt++) 
	{
	//	printf("i = %d, cnt = %d\n",i,cnt);
		i = array[i];
	}


// Get time after
	uint64_t elapsed = getTicks(c1) - before_time; 
	printf("total elapsed = %lu, twototwenty = %lu, average = %lu\n", elapsed,twototwenty,elapsed/twototwenty);

}

