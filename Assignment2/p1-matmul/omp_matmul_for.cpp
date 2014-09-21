#include <omp.h>

void omp_for_matmuld(double **a, double **b, double **c, int nthr)
{
  /* this call is needed to set the number
   * of threads */
  omp_set_num_threads(nthr);

  /* CS194: add pragmas to this loop-nest
   * to enable OpenMP for parallelism */
  int i,j,k;
	#pragma omp parallel for shared(a,b,c) private(i,j,k)
	  	for(i=0;i<1024;i++)
			for(j=0;j<1024;j++)
				for(k=0;k<1024;k++)
					c[i][j] += a[i][k]*b[k][j];
}
