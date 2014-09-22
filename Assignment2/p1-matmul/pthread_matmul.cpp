#include <pthread.h>
#include <cassert>
#include <stdio.h>

typedef struct
{
  double **a;
  double **b;
  double **c;
  int start;
  int end;
} worker_t;

void *matmuld_worker(void *arg)
{
  worker_t *t = static_cast<worker_t*>(arg);
  double **a = t->a;
  double **b = t->b;
  double **c = t->c;
  for(int i = t->start; i < t->end; i++)
    {
      for(int j = 0; j < 1024; j++)
	{
	  for(int k = 0; k < 1024; k++)
	    {
	      c[i][j] += a[i][k]*b[k][j];
	    }
	}
    }
  //printf("test in thread\n");
}

void pthread_matmuld(double **a,
		     double **b,
		     double **c,
		     int nthr)
{
  /* CS194: use pthreads to launch 
   * matrix multply worker threads.
   *
   * The structure and worker function
   * are good hints...
   */
  int t;
  int id;
  pthread_t *thr = new pthread_t[nthr];
  worker_t *tInfo = new worker_t[nthr];

  for(t = 0; t < nthr; t++) {
	  tInfo[t].a = a;
	  tInfo[t].b = b;
	  tInfo[t].c = c;
	  tInfo[t].start = 1024/nthr * t;
	  int end = 1024/nthr * (t+1);

	  if(end + 1024/nthr > 1024)
	  	end = 1024;
	  
	  tInfo[t].end = end;
	  //printf("before pthread create in t:%d\n",t);
	  id = pthread_create(&thr[t], NULL, matmuld_worker, (void *)&(tInfo[t]));
	  //printf("after pthread create in t:%d id:%d\n",t,id);
	  if(id)
	  {
		  printf("ERROR;return code from pthread_create() is %d\n", id);
		 
	  }
  }
  
  for(t = 0; t < nthr; t++) {
	  pthread_join(thr[t],NULL);
  }
  
  delete [] thr;
  delete [] tInfo;
}
