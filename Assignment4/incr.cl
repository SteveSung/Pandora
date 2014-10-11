__kernel void incr (__global float *Y, int n)
{
  // get Global work-item ID value.
  int idx = get_global_id(0);
  if(idx < n)
    {
      // increment execution.
      Y[idx] = Y[idx] + 1.0f;
    }
}
