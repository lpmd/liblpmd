/*
 *
 *
 *
 */

#ifndef __PY_TIMER_H__
#define __PY_TIMER_H__

#include <lpmd/timer.h>

extern "C" PyTypeObject PyTimerType;

//
//
class PyTimer
{
 public:
   PyObject_HEAD
   lpmd::Timer * timer;
};


#endif

