//
//
//

#include <lpmd/paramlist.h>
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

void ParamList::AssignParameter(const std::string & key, std::string value) { (paramlist_impl->innermap)[key] = value; }

std::string & ParamList::operator[](const std::string & key) { return (paramlist_impl->innermap)[key]; }

const std::string & ParamList::operator[](const std::string & key) const
{
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 const std::map<std::string, std::string>::const_iterator & p = im.find(key);
 return (*p).second;
}

void ParamList::Remove(const std::string & key) { (paramlist_impl->innermap).erase(key); }

std::list<std::string> ParamList::Parameters() const
{
 std::list<std::string> tmp;
 std::map<std::string, std::string> & im = paramlist_impl->innermap;
 for (std::map<std::string, std::string>::const_iterator it=im.begin();it != im.end();++it) tmp.push_back(it->first);
 return tmp;
}

