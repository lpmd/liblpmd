/*
 *
 *
 *
 */

#ifndef __LPMD_STEPPER_H__
#define __LPMD_STEPPER_H__

namespace lpmd
{

 class Stepper
 {
  public:
     Stepper(): start(0), end(-1), each(1) { }
     Stepper(long int s, long int e, long int d=1): start(s), end(e), each(d) { };

     inline bool IsActiveInStep(long i)
     {
      if (i < start) return false;
      if ((end != -1) && (i > end)) return false;
      if (((i - start) % each) == 0) return true;
      return false;
     }

     inline double ValueAtStep(long i, double from_v, double to_v)
     {
      if (start == end) return from_v;
      return (from_v + double(i-start)/double(end-start)*(to_v - from_v));
     }

  // 
  long int start, end, each;
 };

} // lpmd

#endif

