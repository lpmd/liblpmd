//
//
//

#ifndef __LPMD_PLUGINMANAGER_H__
#define __LPMD_PLUGINMANAGER_H__

#include <list>
#include <map>
#include <exception>
#include "plugin.h"
#include "paramlist.h"

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
   void DefineAlias(const std::string & id, const std::string & name);

   std::string GetPluginKeywords(std::string name);

   Module & operator[](std::string id); 

 private:
   std::map<std::string, Module *> modules;
   std::list<std::string> pluginpath;
   ParamList aliasdict;
};

class ModuleNotFound: public std::exception
{
 public:
   ModuleNotFound(std::string name);
   ~ModuleNotFound() throw ();

   const char * what() const throw();

 private:
   std::string modname;
};

} // lpmd 

#endif

