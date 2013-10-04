//
//
//

#ifndef __LPMD_TIMER_H__
#define __LPMD_TIMER_H__

namespace lpmd
{

 class Timer
 {
  public:
    //
    Timer();
    ~Timer();

   void Start();
   void Stop();
   double Elapsed() const;

   private:
     class TimerImpl * impl;
 };

} // lpmd

#endif

