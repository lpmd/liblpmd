//
//
//

#ifndef __LPMD_PLUGINMANAGER_H__
#define __LPMD_PLUGINMANAGER_H__

#include <list>
#include <map>

#include <lpmd/plugin.h>
#include <lpmd/paramlist.h>
#include <lpmd/error.h>

namespace lpmd
{

class ModuleInfo
{
 public:
   std::string name;
   std::string id;
   std::string args;

   ModuleInfo(std::string modulename, std::string moduleid, std::string moduleargs);
};


class PluginManager
{
 public:
   //
   PluginManager();
   ~PluginManager();

   void AddToPluginPath(std::string pdir);

   void LoadPluginFile(std::string path, std::string id, std::string args);
   void LoadPlugin(std::string name, std::string id, std::string args);
   void LoadPlugin(std::string name, std::string args);
   void UnloadPlugin(std::string id);
   void DefineAlias(const std::string id, const std::string name);

   std::string GetPluginKeywords(std::string name);
   bool IsProvided(const std::string property);
   Module & Provider(const std::string property);
   std::list<std::string> NamedProperties();

   Module & operator[](std::string id); 

 private:
   std::map<std::string, Module *> modules;
   std::list<std::string> pluginpath;
   std::map<std::string, Module *> namedprops;
   ParamList aliasdict;
};

// Safe casting of Modules into specific kinds of plugins
template<class T> T & CastModule(Module & m)
{
 try { return dynamic_cast<T &>(m); }
 catch (std::exception & ex) { throw InvalidModuleType(); }
}

class ModuleNotFound: public Error
{
 public:
   ModuleNotFound(const std::string name);
};

class PropertyNotFound: public Error
{
 public:
   PropertyNotFound(const std::string name);
};

} // lpmd 

#endif

