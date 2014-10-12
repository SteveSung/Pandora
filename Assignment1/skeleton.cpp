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

// Get time befor
uint64_t time = getTicks(c1);

// Do some computation


// Get time after
uint64_t elapsed = getTicks(c1) - time; 
printf("elapsed = %lu\n", elapsed);

}

