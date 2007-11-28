//
//
//

#include <iostream>
#include <sstream>

#include <lpmd/module.h>
#include <lpmd/util.h>

using namespace lpmd;

Module::Module() 
{ 
 name = "unnamed";
 used = false;
}

Module::Module(std::string modulename) 
{ 
 name = modulename;
 used = false; 
}

Module::~Module() { }

void Module::ProcessArguments(std::string line)
{
 std::string tmp;
 words = ListOfTokens(line);
 while (words.size() > 0)
 {
  tmp = words.front();
  words.pop_front();
  RemoveUnnecessarySpaces(tmp);
  SetParameter(tmp);
 }
}

std::string Module::GetNextWord()
{
 std::string tmp = words.front();
 words.pop_front();
 return tmp;
}

std::string Module::GetNextWord(char d)
{
 char e;
 if(d=='<') e= '>';
 else if(d=='[') e= ']';
 else if(d=='(') e= ')';
 else e=d;

 std::string nextword = words.front();
 words.pop_front();
 if(nextword[0]!=d) {std::cerr << "Error reading module" << '\n';exit(0);}
 if(nextword[0]==d && (nextword[nextword.size()-1]==e && nextword.size()>1)) return nextword;

 std::ostringstream ostr;
 ostr << nextword;
 nextword = " ";
 while(nextword[nextword.size()-1]!=e)
 {
  nextword = words.front();
  ostr <<" "<< nextword;
  words.pop_front();
 }
 return ostr.str();
}

void Module::Show() const
{
 std::cout << "Module " << name << ": " << '\n';
}

void Module::ShowHelp() const
{
 std::cout << "No help available for this module." << '\n';
}

std::string Module::Name() const { return name; }

bool Module::Used() const { return used; }

void Module::SetUsed() { used = true; }

const char * InvalidModuleType::what() const throw()
{
 return "Se intento utilizar un modulo que no tiene las capacidades requeridas (por ejemplo usar un Potential como Integrator)";
}


