/*
 *
 *
 *
 */

#ifndef __LPMD_TASKRETURNVALUE_H__
#define __LPMD_TASKRETURNVALUE_H__

#include <lpmd/returnvalue.h>

namespace lpmd
{

 template <typename T> class TaskReturnValue: public ReturnValue
 {
  public:
    TaskReturnValue(const T & val)
    {
     SetValue(val);
    }

    void SetValue(const T & val)
    {
     innervalue = new T(val);
    }

    const T & GetValue() const
    {
     return *innervalue;
    }

  private:
    const T * innervalue;
 };

} // lpmd

#endif

