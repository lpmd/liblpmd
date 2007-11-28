//
//
//

#ifndef __LPMD_APPLICABLE_H__
#define __LPMD_APPLICABLE_H__

namespace lpmd
{

 class IApplicable
 {
  public:
    //
    IApplicable();
    virtual ~IApplicable();
 
    //
    long int start_step;
    long int end_step;
    long int interval;
 };

} // lpmd

#endif

