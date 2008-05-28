//
//
//

#include <lpmd/plugin.h>

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>

using namespace lpmd;

//
// Dynamic loading of modules
//
Module * lpmd::LoadPluginModule(std::string path, std::string args)
{
 struct stat sst;
 if (stat(path.c_str(), &sst) == -1) return NULL;      // could not 'stat' the plugin file... 
 void * mymodule = dlopen(path.c_str(), RTLD_LAZY);
 if (!mymodule) 
 {
  std::cerr << "[Error] Could not create instance of Module (from plugin module " << path << ")\n";
  std::cerr << dlerror() << '\n';
  exit(1);
 }
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

