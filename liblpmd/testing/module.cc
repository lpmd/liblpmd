//
//
//

#include <lpmd/module.h>
#include <lpmd/pluginmanager.h>
#include <lpmd/util.h>

#include <iostream>
#include <fstream>
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
   std::ostream * debugstr;     // puntero al flujo de depuracion actual
   bool extdebugstr;            // true si es necesario hacer delete sobre debugstr
   bool strictkw;               // true si solo considera como argumentos las palabras devueltas por Keyword()
   std::string kwstr;
   PluginManager * manager;

   ModuleImpl() 
   {
    name = "unnamed";
    used = false;
    emptycall = false;
    debugstr = &(std::cerr);
    extdebugstr = false;
    strictkw = true;
    kwstr = "";
    manager = NULL;
   }
};

Module::Module(const Module & mod) 
{ 
 impl = new ModuleImpl();
 impl->name = mod.Name();
 impl->used = false;
 impl->emptycall = false;
 impl->strictkw = mod.impl->strictkw;
 impl->kwstr = mod.impl->kwstr;
 std::list<std::string> kwd = mod.Parameters();
 for (std::list<std::string>::const_iterator it=kwd.begin();it!=kwd.end();++it) AssignParameter(*it, mod.GetString(*it));
}

Module::Module(std::string modulename, bool strictkw)
{ 
 impl = new ModuleImpl();
 impl->strictkw = strictkw;
 impl->name = modulename;
 impl->used = false; 
 impl->emptycall = false;
}

Module::~Module() 
{ 
 if (impl->extdebugstr) delete impl->debugstr;
 delete impl; 
}

Module & Module::operator=(const Module & mod)
{
 if (&mod != this)
 {
  impl->name = mod.Name();
  impl->used = false;
  impl->emptycall = false;
  impl->strictkw = mod.impl->strictkw;
  impl->kwstr = mod.impl->kwstr;
  std::list<std::string> kwd = mod.Parameters();
  for (std::list<std::string>::const_iterator it=kwd.begin();it!=kwd.end();++it) AssignParameter(*it, mod.GetString(*it));
 }
 return (*this);
}

void Module::ProcessArguments(std::string line)
{
 if (!Defined("debug")) DefineKeyword("debug", "stderr");
 // Registra los parametros que ya tengan valores (por omision)
 std::list<std::string> kwd = Parameters();
 for (std::list<std::string>::const_iterator it=kwd.begin();it!=kwd.end();++it)
     impl->defvalues.AssignParameter(*it, GetString(*it));
 // Asigna los nuevos valores, dados en line
 std::string tmp;
 impl->words = StringSplit< std::list<std::string> >(line);
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

void Module::AssignParameter(const std::string & key, std::string value)
{
 ParamList::AssignParameter(key, value);
 //
 // Inicializa el modulo segun los parametros asignados
 //
 if (key == "debug")
 { 
  const std::string d = GetString("debug");
  if (d == "stdout") SetDebugStream(std::cout);
  else if (d == "stderr") SetDebugStream(std::cerr);
  else if (d == "none") SetDebugFile("/dev/null");
  else SetDebugFile(d);
 }
}

void Module::Show() const { Show(std::cout); }

void Module::Show(std::ostream & os) const 
{ 
 os << "Module " << impl->name;
 if (Defined("module") && (GetString("module") != Name())) os << " (loaded as \"" << GetString("module") << "\")";
 os << ": " << '\n'; 
 std::list<std::string> kwds;
 if (impl->strictkw) kwds = StringSplit< std::list<std::string> >(Keywords());
 else kwds = Parameters();
 for (std::list<std::string>::const_iterator it=kwds.begin();it!=kwds.end();++it)
 {
  if ((*it) == "module") continue;      // No muestra parametro "module", reservado para uso interno
  if ((*it) == "fullpath") continue;  // parametro reservado
  if (impl->emptycall) 
  { 
   os << "   " << std::setw(20) << (*it);
   if (impl->defvalues.Defined(*it)) os << " = " << std::setw(30) << impl->defvalues.GetString(*it) << '\n';
   else os << " has no default value\n";
  }
  else
  {
   os << "   " << std::setw(20) << (*it) << " = " << std::setw(30) << GetString(*it);
   if (impl->defvalues.Defined(*it)) os << " (default: " << impl->defvalues.GetString(*it) << ")\n";
   else os << '\n';
  }
 }
}

void Module::ShowHelp() const
{
 // This is the default implementation, if not overloaded in a plugin
 std::cout << "No help available for this module." << '\n';
}

void Module::DefineKeyword(const std::string kw, const std::string defvalue) 
{ 
 impl->kwstr += (kw+" "); 
 AssignParameter(kw, defvalue);
 impl->defvalues.AssignParameter(kw, defvalue);
}

void Module::DefineKeyword(const std::string kw) { impl->kwstr += (kw+" "); }

std::string Module::Keywords() const { return impl->kwstr; }

void Module::SetDebugStream(std::ostream & ostr) { impl->extdebugstr = false; impl->debugstr = &ostr; }

void Module::SetDebugFile(const std::string debugfile) 
{
 impl->extdebugstr = true;
 impl->debugstr = new std::ofstream(debugfile.c_str());
 (impl->debugstr)->setf(std::ios::unitbuf);
}

std::ostream & Module::DebugStream() const { return *(impl->debugstr); }

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

InvalidModuleType::InvalidModuleType(): Error("Attempted to load module of the wrong type") { }

UnknownProperty::UnknownProperty(const std::string name): Error("Requesting unknown property, \""+name+"\"") { }

