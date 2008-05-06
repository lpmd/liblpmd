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
   void ShowElapsedTimes() const;

   private:
     class TimerImpl * timpl;
 };

} // lpmd

#endif


