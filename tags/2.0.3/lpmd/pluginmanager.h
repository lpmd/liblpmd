//
//
//

#ifndef __LPMD_PLUGINMANAGER_H__
#define __LPMD_PLUGINMANAGER_H__

#include <map>

#include <lpmd/plugin.h>
#include <lpmd/paramlist.h>
#include <lpmd/error.h>
#include <lpmd/moduleinfo.h>

namespace lpmd
{

// Safe casting of Modules into specific kinds of plugins
template<class T> T & CastModule(Module & m)
{
 try { return dynamic_cast<T &>(m); }
 catch (std::exception & ex) { throw InvalidOperation("The requested casting of plugins"); }
}

class PluginManager
{
 public:
   //
   PluginManager();
   ~PluginManager();

   Array<std::string> Plugins() const;

   void AddToPluginPath(std::string pdir);

   void LoadPluginFile(std::string path, std::string id, std::string args);
   void LoadPlugin(std::string name, std::string id, std::string args);
   void LoadPlugin(std::string name, std::string args);
   void LoadPlugin(ModuleInfo info);
   void UnloadPlugin(std::string id);
 
   bool IsLoaded(std::string id); 
   void UpdatePlugin(std::string id, std::string new_args);

   template<typename T> bool HasType(const std::string & id)
   {
    try { T & t = CastModule<T>((*this)[id]); return (&t != 0); }
    catch (InvalidOperation & e) { return false; }
   }

   template<typename T> T & LoadPluginAs(std::string name, std::string args)
   {
    LoadPlugin(name, args);
    return CastModule<T>((*this)[name]);
   }

   void DefineAlias(const std::string id, const std::string name);

   std::string GetPluginKeywords(std::string name);
   bool IsProvided(const std::string property);
   Plugin & Provider(const std::string property);
   Array<std::string> NamedProperties();

   Plugin & operator[] (std::string id); 
   const Plugin & operator[](std::string id) const;

 private:
   std::map<std::string, Plugin *> modules;
   Array<std::string> pluginpath;
   std::map<std::string, Plugin *> namedprops;
   ParamList aliasdict;
};

} // lpmd 

std::ostream & operator<<(std::ostream & os, const lpmd::PluginManager & pm);

#endif

