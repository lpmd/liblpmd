/*
 *
 *
 *
 */

#ifndef __LPMD_TASK_H__
#define __LPMD_TASK_H__

namespace lpmd
{

template <typename T, typename R> class Task
{
 public:

  virtual ~Task() { };

  virtual T InnerFunction() { return pfunc; }
  virtual R Result() { return res; }

 protected:
  T pfunc;
  R res;
};

} // lpmd

#endif

