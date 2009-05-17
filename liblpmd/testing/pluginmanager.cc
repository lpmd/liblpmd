//
//
//

#include <lpmd/pluginmanager.h>
#include <lpmd/util.h>

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
 const std::list<std::string> lst = StringSplit< std::list<std::string> >(path, ':'); 
 for (std::list<std::string>::const_iterator it=lst.begin();it!=lst.end();++it)
 {
  if ((*it) != "") AddToPluginPath(*it);
 }
}

PluginManager::~PluginManager()
{
 std::string name;
 std::list<std::string> kill_list;
 for (std::map<std::string, Module *>::iterator it=modules.begin();it != modules.end();++it) kill_list.push_back(it->first);
 for (std::list<std::string>::iterator it=kill_list.begin();it != kill_list.end();++it)
 {
  name = *it;
  UnloadPlugin(name);
 }
}

void PluginManager::AddToPluginPath(std::string pdir) { pluginpath.push_front(pdir); }

void PluginManager::LoadPluginFile(std::string path, std::string id, std::string args)
{
 Module * mm = LoadPluginModule(path, args);
 if (mm == NULL) throw ModuleNotFound(path);
 mm->SetManager(*this);                      // FIXME: Por ahora se hace aqui, deberia ir en un constructor de Module en el nuevo API
 modules[id] = mm;
 // Agrega nuevas propiedades a la lista
 std::list<std::string> proplist = StringSplit< std::list<std::string> >(mm->Provides());
 for (std::list<std::string>::const_iterator it=proplist.begin();it!=proplist.end();++it) { namedprops[*it] = mm; }
}

void PluginManager::LoadPlugin(std::string name, std::string id, std::string args)
{
 Module * mm = NULL;
 for (std::list<std::string>::const_iterator p=pluginpath.begin();p != pluginpath.end();++p)
 {
  mm = NULL;
  const std::string & plugindir = *p;
  mm = LoadPluginModule(plugindir+"/"+name+".so", args);
  if (mm != NULL) break;
 }
 if (mm == NULL) throw ModuleNotFound(name);
 mm->SetManager(*this);                      // FIXME: Por ahora se hace aqui, deberia ir en un constructor de Module en el nuevo API
 modules[id] = mm;
 // Agrega nuevas propiedades a la lista
 std::list<std::string> proplist = StringSplit< std::list<std::string> >(mm->Provides());
 for (std::list<std::string>::const_iterator it=proplist.begin();it!=proplist.end();++it) { namedprops[*it] = mm; }
}

void PluginManager::LoadPlugin(std::string name, std::string args) { LoadPlugin(name, name, args); }

void PluginManager::UnloadPlugin(std::string id)
{
 Module * m = modules[id];
 modules[id] = NULL;
 if (m != NULL)
 {
  // Elimina las propiedades de la lista
  std::list<std::string> proplist = StringSplit< std::list<std::string> >(m->Provides());
  for (std::list<std::string>::const_iterator it=proplist.begin();it!=proplist.end();++it) { namedprops.erase(*it); }
  delete m;
 }
}

std::string PluginManager::GetPluginKeywords(std::string name)
{
 if (aliasdict.Defined(name)) LoadPlugin(aliasdict[name], "dummyargument");
 else LoadPlugin(name, "dummyargument");
 Module * m = modules[name];
 std::string kw = m->Keywords();
 UnloadPlugin(name);
 return kw;
}

bool PluginManager::IsProvided(const std::string property) { return (namedprops.count(property) > 0); }

Module & PluginManager::Provider(const std::string property)
{
 if (! IsProvided(property)) { throw PropertyNotFound(property); }
 else return *(namedprops[property]);
}

std::list<std::string> PluginManager::NamedProperties()
{
 std::list<std::string> tmp;
 for (std::map<std::string, Module *>::iterator it=namedprops.begin();it!=namedprops.end();++it) { tmp.push_back(it->first); }
 return tmp;
}

void PluginManager::DefineAlias(const std::string id, const std::string name) { aliasdict[id] = name; }

Module & PluginManager::operator[](std::string id) 
{
 if (modules.count(id) == 0) throw ModuleNotFound(id); 
 else return *(modules[id]); 
}

//
//
//

PropertyNotFound::PropertyNotFound(const std::string name): Error("Property \""+name+"\" is not provided by any loaded module.") { }

ModuleNotFound::ModuleNotFound(const std::string name): Error("El modulo \""+name+"\" no existe.") { }

