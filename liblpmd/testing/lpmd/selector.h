/*
 *
 *
 *
 */

#ifndef __LPMD_SELECTOR_H__
#define __LPMD_SELECTOR_H__

namespace lpmd
{

 template <typename T> class Selector
 {
  public:
    virtual ~Selector() { };
    virtual const T & SelectFrom(const T & obj) = 0;
    virtual const T & InverseSelectFrom(const T & obj) = 0;
 };


}  // lpmd

#endif

