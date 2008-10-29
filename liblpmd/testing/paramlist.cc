//
//
//

#include <lpmd/paramlist.h>

#include <iostream>
#include <string>
#include <map>

using namespace lpmd;

class lpmd::ParamListImpl
{
 public:
   std::map<std::string, std::string> innermap;
};

ParamList::ParamList() { paramlist_impl = new ParamListImpl(); }

ParamList::~ParamList() { delete paramlist_impl; }

bool ParamList::Defined(const std::string & key) const 
{ 
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 if (im.count(key) > 0) return true;
 return false;
}

void ParamList::AssignParameter(const std::string & key, std::string value) 
{ 
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 im[key] = value; 
}

std::string ParamList::GetString(const std::string & key) 
{ 
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 return im[key]; 
}

std::string ParamList::GetString(const std::string & key) const
{
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 const std::map<std::string, std::string>::const_iterator & p = im.find(key);
 return (*p).second;
}

double ParamList::GetDouble(const std::string & key) { return atof(GetString(key).c_str()); }

long ParamList::GetInteger(const std::string & key) { return atoi(GetString(key).c_str()); }

bool ParamList::GetBool(const std::string & key) 
{ 
 if (GetString(key) == "true") return true;
 return false;
}

lpmd::Vector ParamList::GetVector(const std::string & key)
{
 lpmd::Vector tmp(0,0,0);
 std::string text = GetString(key);
 RemoveUnnecessarySpaces(text);
 if(text[0]!='<' || text[text.size()-1]!='>')
 {
  std::cerr << "Error during paremeters reading >> GetVector() Error!" << '\n';
  exit (0);
 }
 text.erase(text.begin());
 text.erase(text.end()-1);
 size_t found;
 found=text.find_first_of(",");
 while (found!=std::string::npos)
 {
  text[found]=' ';
  found=text.find_first_of(",",found+1);
 }
 std::istringstream iss(text);
 double a=0.0e0,b=0.0e0,c=0.0e0;
 iss >> a >> b >> c;
 tmp.SetX(a);
 tmp.SetY(b);
 tmp.SetZ(c);
 return tmp;
}

std::string & ParamList::operator[](const std::string & key) 
{
 return (paramlist_impl->innermap).operator[](key);
}

void ParamList::Remove(const std::string & key) { (paramlist_impl->innermap).erase(key); }

std::list<std::string> ParamList::Parameters() const
{
 std::list<std::string> tmp;
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 for (std::map<std::string, std::string>::const_iterator it=im.begin();it != im.end();++it) tmp.push_back(it->first);
 return tmp;
}

std::ostream & lpmd::operator<<(std::ostream & os, const ParamList & pl)
{
 std::list<std::string> params = pl.Parameters();
 for (std::list<std::string>::const_iterator it=params.begin();it != params.end();++it)
   os << (*it) << " = " << pl.GetString(*it) << '\n';
 return os;
}


