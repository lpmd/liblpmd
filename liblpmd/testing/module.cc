//
//
//

#include <lpmd/module.h>
#include <lpmd/pluginmanager.h>
#include <lpmd/util.h>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace lpmd;

class lpmd::ModuleImpl
{
 public:
   bool used;
   std::list<std::string> words;
   ParamList defvalues;
   std::string name;
   bool emptycall;
   PluginManager * manager;

   ModuleImpl() 
   {
    name = "unnamed";
    used = false;
    emptycall = false;
    manager = NULL;
   }
};

Module::Module() { impl = new ModuleImpl(); }

Module::Module(const Module & mod) 
{ 
 impl = new ModuleImpl();
 impl->name = mod.Name();
 impl->used = false;
 impl->emptycall = false;
 std::list<std::string> kwd = mod.Parameters();
 for (std::list<std::string>::const_iterator it=kwd.begin();it!=kwd.end();++it) AssignParameter(*it, mod.GetString(*it));
}

Module::Module(std::string modulename) 
{ 
 impl = new ModuleImpl();
 impl->name = modulename;
 impl->used = false; 
 impl->emptycall = false;
}

Module::~Module() { delete impl; }

Module & Module::operator=(const Module & mod)
{
 if (&mod != this)
 {
  impl->name = mod.Name();
  impl->used = false;
  impl->emptycall = false;
  std::list<std::string> kwd = mod.Parameters();
  for (std::list<std::string>::const_iterator it=kwd.begin();it!=kwd.end();++it) AssignParameter(*it, mod.GetString(*it));
 }
 return (*this);
}

void Module::ProcessArguments(std::string line)
{
 // Registra los parametros que ya tengan valores (por omision)
 std::list<std::string> kwd = Parameters();
 for (std::list<std::string>::const_iterator it=kwd.begin();it!=kwd.end();++it)
     impl->defvalues.AssignParameter(*it, GetString(*it));
 // Asigna los nuevos valores, dados en line
 std::string tmp;
 impl->words = ListOfTokens(line);
 if (impl->words.size() == 0) impl->emptycall = true;
 else
 {
  while (impl->words.size() > 0)
  {
   tmp = impl->words.front();
   impl->words.pop_front();
   RemoveUnnecessarySpaces(tmp);
   SetParameter(tmp);
  }
  impl->emptycall = false;
 }
}

std::string Module::GetNextWord()
{
 if (impl->words.size() > 0)
 {
  std::string tmp = impl->words.front();
  impl->words.pop_front();
  return tmp;
 } 
 return "(null)";
}

std::string Module::GetNextWord(char d)
{
 char e;
 if(d=='<') e= '>';
 else if(d=='[') e= ']';
 else if(d=='(') e= ')';
 else e=d;

 std::string nextword = impl->words.front();
 impl->words.pop_front();
 if(nextword[0]!=d) {std::cerr << "Error reading module" << '\n';exit(0);}
 if(nextword[0]==d && (nextword[nextword.size()-1]==e && nextword.size()>1)) return nextword;

 std::ostringstream ostr;
 ostr << nextword;
 nextword = " ";
 while(nextword[nextword.size()-1]!=e)
 {
  nextword = impl->words.front();
  ostr <<" "<< nextword;
  impl->words.pop_front();
 }
 return ostr.str();
}

void Module::SetParameter(std::string name) { AssignParameter(name, GetNextWord()); }

void Module::Show() const { Show(std::cout); }

void Module::Show(std::ostream & os) const 
{ 
 os << "Module " << impl->name << ": " << '\n'; 
 std::list<std::string> kwds = Parameters();
 for (std::list<std::string>::const_iterator it=kwds.begin();it!=kwds.end();++it)
 {
  if (*it == "module") continue;    // parametro reservado
  if (*it == "fullpath") continue;  // parametro reservado
  if (impl->emptycall) 
  { 
   os << "   " << std::setw(10) << (*it);
   if (impl->defvalues.Defined(*it)) os << " = " << std::setw(20) << impl->defvalues.GetString(*it) << '\n';
   else os << " has no default value\n";
  }
  else
  {
   os << "   " << std::setw(10) << (*it) << " = " << std::setw(20) << GetString(*it);
   if (impl->defvalues.Defined(*it)) os << " (default value: " << impl->defvalues.GetString(*it) << ")\n";
   else os << " (no default value)\n";
  }
 }
}

void Module::ShowHelp() const
{
 // This is the default implementation, if not overloaded in a plugin
 std::cout << "No help available for this module." << '\n';
}

std::string Module::Provides() const 
{ 
 // This is the default implementation, if not overloaded in a plugin
 return ""; 
}

double Module::GetProperty(const std::string & name)
{
 std::cerr << "[Warning] The module \"" << name << "\" does not implement access to any properties." << '\n';
 return 0.0;
}

std::string Module::Name() const { return impl->name; }

bool Module::Used() const { return impl->used; }

void Module::SetUsed() { impl->used = true; }

void Module::SetManager(PluginManager & pm) { impl->manager = &pm; }

PluginManager & Module::GetManager() const 
{
 if (impl->manager == NULL) throw PluginError(impl->name, "Called GetManager() on an unmanaged plugin");
 return *(impl->manager); 
}

//
//
//

InvalidModuleType::InvalidModuleType(): Error("Attempted to load module of the wrong type") { }

UnknownProperty::UnknownProperty(const std::string name): Error("Requesting unknown property, \""+name+"\"") { }

