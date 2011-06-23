/*
 *
 *
 *
 */

#ifndef __LPMD_TASKRUNNER_H__
#define __LPMD_TASKRUNNER_H__

#include <lpmd/task.h>

namespace lpmd
{

// TaskRunner es una interfaz que todo 
// objeto que ejecute una Task debe implementar
template <typename T, typename R> class TaskRunner
{
 public:

   virtual ~TaskRunner() { }

   virtual void RunTask(Task<T, R> & t) = 0;

};

} // lpmd

#endif

