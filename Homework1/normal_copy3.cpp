#include <emmintrin.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <sys/time.h>
#include <time.h>



#define N 8192   //32KB
#define N2 16384  //64KB
#define N3 32768 //128KB
#define N4 65536 //256KB
#define N5 131072//512KB
#define N6 262144 //1MB
#define N7 524288 //2MB
#define N8 1048579 //4MB
#define N9 2097152 //5MB


void cpy_array1(int *dst, int *src, int n) {
	for(int i=0; i< n; i++)
		dst[i] = src[i];
}

void cpy_array2(int *dst, int *src, int n) {
	for(int i=0; i< n; i+=4) {
		dst[i] = src[i];
		dst[i+1] = src[i+1];
		dst[i+2] = src[i+2];
		dst[i+3] = src[i+3];
	}
}

void cpy_array3(int *dst, int *src) {
	*dst = *src;
}

int main(int argc, char *argv[])
{
  struct timeval tv;
  struct timeval tv2;
  
	for(int k=1; k<= N9 ; k=k*2) {
 		int myarray[N*k];
	 	for(int i=0; i< N*k; i++)
			myarray[i] = i;
  
 	 	int copiedarray[N*k];
 	 	double time, time2, time3, bps, Mbps, totalMbps=0;
  		
		for(int h=0; h<N; h++)
			cpy_array3(copiedarray+h, myarray+h);
    
  
  		for(int j=0; j<10; j++) {
  			gettimeofday(&tv ,0);
			for(int h=0; h<N; h++)
  				cpy_array3(copiedarray+h, myarray+h);
  			gettimeofday(&tv2 ,0);
  			double time = tv.tv_sec + 1e-6 * tv.tv_usec;
  			double time2 = tv2.tv_sec + 1e-6 * tv2.tv_usec;
  
			//for(int i=0; i<10; i++)
   				//printf("copied[%d] = %d\n", i, copiedarray[i]);

  			//printf("time = %fns\n", (time2-time)*1000000000);
  			time3 = time2 - time;
  			bps = (N*k*sizeof(int)*8)/time3; 
  			Mbps = bps/(1024*1024);
  
			totalMbps += Mbps;
  			//printf("Mbps = %f\n", Mbps);
  		}
  		printf("%d, %dkb average = %f\n",N*k,k*N/32/4/2,totalMbps/10);
 	 }	
}
