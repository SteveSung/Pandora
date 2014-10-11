__kernel void vvadd (__global float *Y, __global float *A, __global float *B, 
	 int n)
{
    int idx = get_global_id(0);
  if(idx < n)
    {
      // Addition executed.
      Y[idx] = A[idx] + B[idx];
    }
}
