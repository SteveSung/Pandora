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

void simd_memcpy(void *dst, void *src, size_t nbytes)
{
  size_t i;

  size_t ilen = nbytes/sizeof(int);
  size_t ilen_sm = ilen - ilen%16;

  char *cdst=(char*)dst;
  char *csrc=(char*)src;

  int * idst=(int*)dst;
  int * isrc=(int*)src;

  __m128i l0,l1,l2,l3;

  _mm_prefetch((__m128i*)&isrc[0], _MM_HINT_NTA);
  _mm_prefetch((__m128i*)&isrc[4], _MM_HINT_NTA);
  _mm_prefetch((__m128i*)&isrc[8], _MM_HINT_NTA);
  _mm_prefetch((__m128i*)&isrc[12], _MM_HINT_NTA);
  
  for(i=0;i<ilen_sm;i+=16)
    {
      l0 =  _mm_load_si128((__m128i*)&isrc[i+0]);
      l1 =  _mm_load_si128((__m128i*)&isrc[i+4]);
      l2 =  _mm_load_si128((__m128i*)&isrc[i+8]);
      l3 =  _mm_load_si128((__m128i*)&isrc[i+12]);
    
      _mm_prefetch((__m128i*)&isrc[i+16], _MM_HINT_NTA);
      _mm_prefetch((__m128i*)&isrc[i+20], _MM_HINT_NTA);
      _mm_prefetch((__m128i*)&isrc[i+24], _MM_HINT_NTA);
      _mm_prefetch((__m128i*)&isrc[i+28], _MM_HINT_NTA);

      _mm_stream_si128((__m128i*)&idst[i+0],  l0);
      _mm_stream_si128((__m128i*)&idst[i+4],  l1);
      _mm_stream_si128((__m128i*)&idst[i+8],  l2);
      _mm_stream_si128((__m128i*)&idst[i+12], l3);

    }

  for(i=ilen_sm;i<ilen;i++)
    {
      idst[i] = isrc[i];
    }

  for(i=(4*ilen);i<nbytes;i++)
    {
      cdst[i] = csrc[i];
    }
}

void simd_memcpy_cache(void *dst, void *src, size_t nbytes)
{
  size_t i;
  size_t sm = nbytes - nbytes%sizeof(int);
  size_t ilen = nbytes/sizeof(int);
  size_t ilen_sm = ilen - ilen%16;

  char *cdst=(char*)dst;
  char *csrc=(char*)src;

  int * idst=(int*)dst;
  int * isrc=(int*)src;

  __m128i l0,l1,l2,l3;

  _mm_prefetch((__m128i*)&isrc[0], _MM_HINT_T0);
  _mm_prefetch((__m128i*)&isrc[4], _MM_HINT_T0);
  _mm_prefetch((__m128i*)&isrc[8], _MM_HINT_T0);
  _mm_prefetch((__m128i*)&isrc[12], _MM_HINT_T0);
  
  for(i=0;i<ilen_sm;i+=16)
    {
      l0 =  _mm_load_si128((__m128i*)&isrc[i+0]);
      l1 =  _mm_load_si128((__m128i*)&isrc[i+4]);
      l2 =  _mm_load_si128((__m128i*)&isrc[i+8]);
      l3 =  _mm_load_si128((__m128i*)&isrc[i+12]);
    
      _mm_prefetch((__m128i*)&isrc[i+16], _MM_HINT_T0);
      _mm_prefetch((__m128i*)&isrc[i+20], _MM_HINT_T0);
      _mm_prefetch((__m128i*)&isrc[i+24], _MM_HINT_T0);
      _mm_prefetch((__m128i*)&isrc[i+28], _MM_HINT_T0);

      _mm_store_si128((__m128i*)&idst[i+0],  l0);
      _mm_store_si128((__m128i*)&idst[i+4],  l1);
      _mm_store_si128((__m128i*)&idst[i+8],  l2);
      _mm_store_si128((__m128i*)&idst[i+12], l3);

    }

  for(i=ilen_sm;i<ilen;i++)
    {
      idst[i] = isrc[i];
    }

  for(i=(ilen*4);i<nbytes;i++)
    {
      cdst[i] = csrc[i];
    }
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
  		
		simd_memcpy_cache(copiedarray, myarray, N*k*sizeof(int));
  		simd_memcpy_cache(copiedarray, myarray, N*k*sizeof(int));
  
  
  		for(int j=0; j<10; j++) {
  			gettimeofday(&tv ,0);
  			simd_memcpy_cache(copiedarray, myarray, N*k*sizeof(int));
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
