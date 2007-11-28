//
//
//

#include <iostream>
#include <cstdlib>

#include <lpmd/pluginmanager.h>
#include <lpmd/util.h>

#include "defaultpath.h"

using namespace lpmd;

ModuleInfo::ModuleInfo(std::string modulename, std::string moduleid, std::string moduleargs)
{
 name = modulename;
 id = moduleid;
 args = moduleargs;
}

PluginManager::PluginManager() 
{ 
 // Inicializa el pluginpath 
 std::string path;
 const char * pathvariable = getenv("LPMD_PATH");
 if (pathvariable != NULL) path = path + std::string(pathvariable);
 path = path + ":" + LPMD_DEFAULTPATH;
 const std::list<std::string> lst = ListOfTokens(path, ':'); 
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
 modules[id] = mm;
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
 modules[id] = mm;
}

void PluginManager::LoadPlugin(std::string name, std::string args) { LoadPlugin(name, name, args); }

void PluginManager::UnloadPlugin(std::string id)
{
 Module * m = modules[id];
 modules[id] = NULL;
 delete m;
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

void PluginManager::DefineAlias(const std::string & id, const std::string & name)
{
 aliasdict[id] = name;
}

Module & PluginManager::operator[](std::string id)
{
 return *(modules[id]);
}

//
//
//

ModuleNotFound::ModuleNotFound(std::string name) 
{
 modname = name;
}

ModuleNotFound::~ModuleNotFound() throw () { }

const char * ModuleNotFound::what() const throw()
{
 std::string str = "El modulo " + modname + " no existe.";
 return str.c_str();
}


