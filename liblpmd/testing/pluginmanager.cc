//
//
//

#include <lpmd/pluginmanager.h>
#include <lpmd/util.h>
#include "config.h"

#include <iostream>
#include <cstdlib>

using namespace lpmd;

PluginManager::PluginManager() 
{ 
 // Inicializa el pluginpath 
 std::string path;
 const char * pathvariable = getenv("LPMD_PATH");
 if (pathvariable != NULL) path = path + std::string(pathvariable);
 path = path + ":" + PLUGINPATH;
 const Array<std::string> lst = StringSplit(path, ':'); 
 for (long int i=0;i<lst.Size();++i)
 {
  if (lst[i] != "") AddToPluginPath(lst[i]);
 }
}

PluginManager::~PluginManager()
{
 Array<std::string> kill_list;
 for (std::map<std::string, Module *>::iterator it=modules.begin();it != modules.end();++it) kill_list.Append(it->first);
 for (long int i=0;i<kill_list.Size();++i) UnloadPlugin(kill_list[i]);
}

Array<std::string> PluginManager::Plugins() const
{
 Array<std::string> pluginlist;
 for (std::map<std::string, Module *>::const_iterator it=modules.begin();it!=modules.end();++it)
     pluginlist.Append((*it).first);
 return pluginlist;
}

void PluginManager::AddToPluginPath(std::string pdir) { pluginpath.Append(pdir); }

void PluginManager::LoadPluginFile(std::string path, std::string id, std::string args)
{
 Module * mm = LoadPluginModule(path, args);
 if (mm == NULL) throw InvalidRequest("Plugin at "+path);
 mm->SetManager(*this);                      // FIXME: Por ahora se hace aqui, deberia ir en un constructor de Module en el nuevo API
 modules[id] = mm;
 // Agrega nuevas propiedades a la lista
 Array<std::string> proplist = StringSplit(mm->Provides());
 for (long int i=0;i<proplist.Size();++i) { namedprops[proplist[i]] = mm; }
}

void PluginManager::LoadPlugin(std::string name, std::string id, std::string args)
{
 Module * mm = NULL;
 for (long int i=0;i<pluginpath.Size();++i)
 {
  mm = NULL;
  const std::string & plugindir = pluginpath[i];
  mm = LoadPluginModule(plugindir+"/"+name+".so", args);
  if (mm != NULL) break;
 }
 if (mm == NULL) throw InvalidRequest("Plugin "+name);
 mm->SetManager(*this);                      // FIXME: Por ahora se hace aqui, deberia ir en un constructor de Module en el nuevo API
 modules[id] = mm;
 DefineAlias(id, name);
 // Agrega nuevas propiedades a la lista
 Array<std::string> proplist = StringSplit(mm->Provides());
 for (long int i=0;i<proplist.Size();++i) { namedprops[proplist[i]] = mm; }
}

void PluginManager::LoadPlugin(std::string name, std::string args) { LoadPlugin(name, name, args); }

void PluginManager::LoadPlugin(ModuleInfo info) { LoadPlugin(info.name, info.id, info.args); }

void PluginManager::UnloadPlugin(std::string id)
{
 Module * m = modules[id];
 modules.erase(id);
 if (m != NULL)
 {
  // Elimina las propiedades de la lista
  Array<std::string> proplist = StringSplit(m->Provides());
  for (long int i=0;i<proplist.Size();++i) { namedprops.erase(proplist[i]); }
  delete m;
 }
}

bool PluginManager::IsLoaded(std::string id) { return (modules.count(id) > 0); }

void PluginManager::UpdatePlugin(std::string id, std::string new_args)
{
 std::string realname = id;
 if (aliasdict.Defined(id)) realname = aliasdict[id];
 UnloadPlugin(id);
 LoadPlugin(realname, id, new_args);
}

std::string PluginManager::GetPluginKeywords(std::string name)
{
 if (aliasdict.Defined(name)) LoadPlugin(aliasdict[name], "tmp_getpluginkeywords", "dummyargument");
 else LoadPlugin(name, "tmp_getpluginkeywords", "dummyargument");
 Module * m = modules["tmp_getpluginkeywords"];
 std::string kw = m->Keywords();
 UnloadPlugin("tmp_getpluginkeywords");
 return kw;
}

bool PluginManager::IsProvided(const std::string property) { return (namedprops.count(property) > 0); }

Module & PluginManager::Provider(const std::string property)
{
 if (! IsProvided(property)) { throw InvalidRequest("Property "+property); }
 else return *(namedprops[property]);
}

Array<std::string> PluginManager::NamedProperties()
{
 Array<std::string> tmp;
 for (std::map<std::string, Module *>::iterator it=namedprops.begin();it!=namedprops.end();++it) { tmp.Append(it->first); }
 return tmp;
}

void PluginManager::DefineAlias(const std::string id, const std::string name) { aliasdict[id] = name; }

Module & PluginManager::operator[](std::string id)
{
 if (modules.count(id) == 0) throw InvalidRequest("Plugin "+id);
 else return *(modules[id]); 
}

const Module & PluginManager::operator[](std::string id) const
{
 if (modules.count(id) == 0) throw InvalidRequest("Plugin "+id);
 const std::map<std::string, Module *>::const_iterator & p = modules.find(id);
 return *((*p).second);
}

std::ostream & operator<<(std::ostream & os, const lpmd::PluginManager & pm)
{
 const Array<std::string> plugins = pm.Plugins();
 for (int i=0;i<plugins.Size();++i)
 {
  std::string pname = plugins[i];
  os << "Plugin " << pname << " -> ";
  os << pm[pname].Name() << " loaded.\n";
  pm[pname].Show(os);
 }
 return os;
}

