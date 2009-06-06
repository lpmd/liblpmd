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
Plugin * lpmd::PluginLoader(std::string path, std::string args)
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
 Plugin * tmp = create_module(args);
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

//
//
//
Plugin::Plugin(const std::string & pluginname, const std::string & pluginversion): Module(pluginname), used(false)
{ 
 AssignParameter("apirequired", "2.0");
 AssignParameter("version", pluginversion);
}

Plugin::Plugin(const std::string & pluginname, const std::string & pluginversion, const std::string & bugreport): Module(pluginname), used(false)
{ 
 AssignParameter("apirequired", "2.0");
 AssignParameter("version", pluginversion);
 AssignParameter("bugreport", bugreport);
}

Plugin::Plugin(const Plugin & mod): Module(mod), used(mod.used) { } 

Plugin::~Plugin() { }

bool Plugin::Used() const { return used; }

void Plugin::SetUsed() { used = true; }

bool Plugin::AutoTest() 
{ 
 throw PluginError(Name(), "Autotest not implemented.");
}

void Plugin::CheckConsistency() { }

