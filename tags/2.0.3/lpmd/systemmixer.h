//
//
//

#ifndef __LPMD_SYSTEMMIXER_H__
#define __LPMD_SYSTEMMIXER_H__

namespace lpmd
{
 class Configuration;     // forward
 class SystemMixer
 {
  public:
   //
   SystemMixer();
   virtual ~SystemMixer();

   virtual Configuration & Apply(Configuration & config1, Configuration & config2) = 0;
 };

} // lpmd 

#endif


