//
//
//

#ifndef __LPMD_PLUGIN_H__
#define __LPMD_PLUGIN_H__

#include <lpmd/module.h>

// Prototype for function
template<class TYPE> TYPE function_cast(void * symbol);

typedef lpmd::Module * create_t(std::string args);
typedef void destroy_t(lpmd::Module *);

extern "C" lpmd::Module * create(std::string args);
extern "C" void destroy(lpmd::Module * m);

namespace lpmd
{

Module * LoadPluginModule(std::string path, std::string args);

} // lpmd

#endif


