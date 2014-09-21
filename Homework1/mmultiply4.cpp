#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include "counters.h"
#include <sys/time.h>

void opt_simd_sgemm(float *Y, float *A, float *B, int n);
void opt_scalar1_sgemm(float *Y, float *A, float *B, int n);
void opt_scalar0_sgemm(float *Y, float *A, float *B, int n);
void naive_sgemm(float *Y, float *A, float *B, int n);

int main(int argc, char *argv[])
{
  int n = (1<<10);
  float* A = new float[n*n];
  float* B = new float[n*n];
  float* Y = new float[n*n];
 
  // init c1 to time the ticks.
  hwCounter_t c1;
  c1.init = false;
  initTicks(c1);

  // init c to count the instructions.
  hwCounter_t c;
  c.init = false;
  initInsns(c);

  struct timeval tv1, tv2;

  // Get before time.
  gettimeofday(&tv1,0);
  // Get before ticks.
  uint64_t tick = getTicks(c1);
  // Get befor instructions.
  uint64_t count = getInsns(c);

  // #####################execution.##########################
  naive_sgemm(Y, A, B, n);
  // Subtract before instructions from after instructions.
  uint64_t executed = getInsns(c) - count;
  // Subtract before time from after ticks.
  uint64_t elapsed = getTicks(c1) - tick;
  // Get after time.
  gettimeofday(&tv2,0);

  double time = (tv2.tv_sec + 1e-6 * tv2.tv_usec) - (tv1.tv_sec + 1e-6 *
		  tv2.tv_usec);

  double Flops = (double)elapsed / (double)time;
  double IPC = (double)executed / (double)elapsed;
  printf("naive_sgemm; instruction : %lu, cycles : %lu, time : %f\nFlops : %lf, IPC : %lf\n",executed, elapsed,
		  time, Flops, IPC);

  delete [] A;
  delete [] B;
  delete [] Y;
}
