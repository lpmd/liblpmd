//
//
//

#ifndef __LPMD_COLORHANDLER_H__
#define __LPMD_COLORHANDLER_H__

#include <lpmd/color.h>

namespace lpmd
{
 class BasicAtom; // forward

 class ColorHandler
 {
  public:
    static bool HaveColor(const BasicAtom & at); 
    static Color & ColorOfAtom(const BasicAtom & at);

  private:
    ColorHandler();  // private constructor
   
    static class ColorHandlerImpl impl;
 };


} // lpmd

#endif
