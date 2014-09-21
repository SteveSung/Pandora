#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include "counters.h"

hwCounter_t c;
c.init = false;
initInsns(c);

uint64_t count = getInsns(c);


uint64_t executed = getInsns(c) - count;
