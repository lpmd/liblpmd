//
//
//

#ifndef __LPMD_SESSION_H__
#define __LPMD_SESSION_H__

#include <lpmd/module.h>

namespace lpmd
{

 class Session: public Module
 {
  public:
    //
    Session();
    ~Session();

   private:
     class SessionImpl * simpl;
 };

 //
 //
 //
 extern Session GlobalSession;

} // lpmd

#endif

