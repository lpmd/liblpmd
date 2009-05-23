//
//
//

#include <lpmd/module.h>
#include <lpmd/pluginmanager.h>
#include <lpmd/util.h>
#include <lpmd/error.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace lpmd;

class lpmd::ModuleImpl
{
 public:
   bool used;
   Array<std::string> words;
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
 Array<Parameter> kwd = mod.Parameters();
 for (long int i=0;i<kwd.Size();++i) AssignParameter(kwd[i], mod[kwd[i]]);
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
  Array<Parameter> kwd = mod.Parameters();
  for (long int i=0;i<kwd.Size();++i) AssignParameter(kwd[i], mod[kwd[i]]);
 }
 return (*this);
}

void Module::ProcessArguments(std::string line)
{
 if (!Defined("debug")) DefineKeyword("debug", "stderr");
 // Registra los parametros que ya tengan valores (por omision)
 Array<Parameter> kwd = Parameters();
 for (long int i=0;i<kwd.Size();++i)
 {
  impl->defvalues[kwd[i]] = (*this)[kwd[i]];
 }
 // Asigna los nuevos valores, dados en line
 std::string tmp;
 impl->words = StringSplit(line);
 if (impl->words.Size() == 0) impl->emptycall = true;
 else
 {
  while (impl->words.Size() > 0)
  {
   tmp = impl->words[0];
   impl->words.Delete(0);
   RemoveUnnecessarySpaces(tmp);
   SetParameter(tmp);
  }
  impl->emptycall = false;
 }
}

std::string Module::GetNextWord()
{
 if (impl->words.Size() > 0)
 {
  std::string tmp = impl->words[0];
  impl->words.Delete(0);
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

 std::string nextword = impl->words[0];
 impl->words.Delete(0);
 if(nextword[0]!=d) {std::cerr << "Error reading module" << '\n';exit(0);}
 if(nextword[0]==d && (nextword[nextword.size()-1]==e && nextword.size()>1)) return nextword;

 std::ostringstream ostr;
 ostr << nextword;
 nextword = " ";
 while(nextword[nextword.size()-1]!=e)
 {
  nextword = impl->words[0];
  ostr <<" "<< nextword;
  impl->words.Delete(0);
 }
 return ostr.str();
}

void Module::SetParameter(std::string name) { AssignParameter(name, GetNextWord()); }

void Module::AssignParameter(const std::string & key, std::string value)
{
 (*this)[key] = value;
 //
 // Inicializa el modulo segun los parametros asignados
 //
 if (key == "debug")
 { 
  const std::string d = (*this)["debug"];
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
 if (Defined("module") && ((*this)["module"] != Name())) os << " (loaded as \"" << (*this)["module"] << "\")";
 os << ": " << '\n'; 
 Array<Parameter> kwds;
 if (impl->strictkw) 
 {
  // FIXME: Define new operator= in Array<Parameter> 
  Array<std::string> tmp = StringSplit(Keywords());
  for (int q=0;q<tmp.Size();++q) kwds.Append(Parameter(tmp[q]));
 }
 else kwds = Parameters();
 for (long int i=0;i<kwds.Size();++i)
 {
  if ((kwds[i]) == "module") continue;      // No muestra parametro "module", reservado para uso interno
  if ((kwds[i]) == "fullpath") continue;  // parametro reservado
  if (impl->emptycall) 
  { 
   os << "   " << std::setw(20) << (kwds[i]);
   if (impl->defvalues.Defined(kwds[i])) os << " = " << std::setw(30) << impl->defvalues[kwds[i]] << '\n';
   else os << " has no default value\n";
  }
  else
  {
   os << "   " << std::setw(20) << (kwds[i]) << " = " << std::setw(30) << (*this)[kwds[i]];
   if (impl->defvalues.Defined(kwds[i])) os << " (default: " << impl->defvalues[kwds[i]] << ")\n";
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
 impl->defvalues[kw] = defvalue;
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

