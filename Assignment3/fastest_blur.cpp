#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <emmintrin.h>
#include <omp.h>

using namespace std;

double timestamp()
{
  struct timeval tv;
  gettimeofday (&tv, 0);
  return tv.tv_sec + 1e-6*tv.tv_usec;
}

// Simple Blur
void simple_blur(float* out, int n, float* frame, int* radii){
  for(int r=0; r<n; r++)
    for(int c=0; c<n; c++){
      int rd = radii[r*n+c];
      int num = 0;
      float avg = 0;
      for(int r2=max(0,r-rd); r2<=min(n-1, r+rd); r2++)
        for(int c2=max(0, c-rd); c2<=min(n-1, c+rd); c2++){
          avg += frame[r2*n+c2];
          num++;
        }
      out[r*n+c] = avg/num;
    }
}

// My Blur
void my_blur(float* out, int n, float* frame, int* radii){
  omp_set_num_threads(16);
  int r,c;
  #pragma omp parallel for private(c) schedule(dynamic)
  for (r = 0; r < n; r++)
    for (c = 0; c < n; c++){
      int rd = radii[r*n+c];
      int num = 0;
      float avg = 0;
      float avg_array[4];
      __m128 temp = _mm_setzero_ps();

      for (int r2 = max(0, r-rd); r2 <= min(n-1, r+rd); r2++)
      {
        int c2;
        for (c2 = max(0, c-rd); c2+4 <= min(n-1, c+rd); c2 += 4) {
          __m128 vector_a = _mm_loadu_ps(frame+(r2*n+c2));
          temp = _mm_add_ps(vector_a, temp);
          num+= 4;
        }

        for (;c2 <= min(n-1, c+rd); c2++) {
          avg += frame[r2*n+c2];
          num++;
        }
      }

      _mm_storeu_ps(avg_array, temp);
      avg = avg + avg_array[0] + avg_array[1] + avg_array[2] + avg_array[3];

      out[r*n+c] = avg/num;
    }
}

int main(int argc, char *argv[])
{
  //Generate random radii
  srand(0);
  int n = 3000;
  int* radii = new int[n*n];
  for(int i=0; i<n*n; i++)
    radii[i] = 6*i/(n*n) + rand()%6;

  //Generate random frame
  float* frame = new float[n*n];
  for(int i=0; i<n*n; i++)
    frame[i] = rand()%256;

  //Blur using simple blur
  float* out = new float[n*n];
  double time = timestamp();
  simple_blur(out, n, frame, radii);
  time = timestamp() - time;

  //Blur using your blur
  float* out2 = new float[n*n];
  double time2 = timestamp();
  my_blur(out2, n, frame, radii);
  time2 = timestamp() - time2;

  //Check result
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++){
      float dif = out[i*n+j] - out2[i*n+j];
      if(dif*dif>1.0f){
        printf("Your blur does not give the right result!\n");
        printf("For element (row, column) = (%d, %d):\n", i, j);
        printf("  Simple blur gives %.2f\n", out[i*n+j]);
        printf("  Your blur gives %.2f\n", out2[i*n+j]);
        exit(-1);
      }
  }

  //Delete
  delete[] radii;
  delete[] frame;
  delete[] out;
  delete[] out2;

  //Print out Time
  printf("Time needed for naive blur = %.3f seconds.\n", time);
  printf("Time needed for your blur = %.3f seconds.\n", time2);
}
