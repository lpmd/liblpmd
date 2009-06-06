//
//
//

#ifndef __LPMD_PLUGIN_H__
#define __LPMD_PLUGIN_H__

#include <lpmd/module.h>

namespace lpmd
{

class Plugin: public Module
{
 public:
   Plugin(const std::string & pluginname, const std::string & pluginversion);
   Plugin(const std::string & pluginname, const std::string & pluginversion, const std::string & bugreport);
   Plugin(const Plugin & mod);
   virtual ~Plugin();

   bool Used() const;
   void SetUsed();

   virtual bool AutoTest();
   virtual void CheckConsistency();

   void (*unloader)(Plugin *);

 private: 
   bool used;
};

Plugin * PluginLoader(std::string path, std::string args);

}  // lpmd

template<class TYPE> TYPE function_cast(void * symbol);

typedef lpmd::Plugin * create_t(std::string args);
typedef void destroy_t(lpmd::Plugin *);

extern "C" lpmd::Plugin * create(std::string args);
extern "C" void destroy(lpmd::Plugin * m);

#endif

