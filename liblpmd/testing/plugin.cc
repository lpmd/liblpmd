//
//
//

#include <lpmd/plugin.h>

#include <dlfcn.h>
#include <cassert>
#include <iostream>
#include <string>

using namespace lpmd;

//
// Dynamic loading of modules
//
Module * lpmd::LoadPluginModule(std::string path, std::string args)
{
 void * mymodule = dlopen(path.c_str(), RTLD_LAZY);
 if (!mymodule) { return NULL; }
 dlerror();
 create_t * create_module = function_cast<create_t *>(dlsym(mymodule, "create"));
 const char *dlsym_error = dlerror();
 if (dlsym_error) 
 {
  std::cerr << "[Error] Could not load symbol: create (in plugin module " << path << ")\n";
  std::cerr << dlerror() << '\n';
  exit(1);
 }
 destroy_t * destroy_module = function_cast<destroy_t *>(dlsym(mymodule, "destroy"));
 dlsym_error = dlerror();
 if (dlsym_error) 
 {
  std::cerr << "[Error] Could not load symbol: destroy (in plugin module " << path << ")\n";
  std::cerr << dlerror() << '\n';
  exit(1);
 }
 Module * tmp = create_module(args);
 tmp->unloader = destroy_module;
 tmp->AssignParameter("fullpath", path);
 return tmp;
}

//
template<class TYPE> TYPE function_cast(void * symbol)
{
 assert(sizeof(void *) == sizeof(TYPE));
 union
 {
  void * symbol;
  TYPE function;
 } cast;
 cast.symbol = symbol;
 return cast.function;
}

PluginError::PluginError(const std::string pluginname, const std::string message): Error("(in plugin "+pluginname+"): "+message) { }

