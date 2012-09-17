//
//
//

#include <lpmd/paramlist.h>
#include <map>

using namespace lpmd;

class lpmd::ParamListImpl
{
 public:
   std::map<std::string, Parameter> innermap;
};

ParamList::ParamList() { paramlist_impl = new ParamListImpl(); }

ParamList::ParamList(const ParamList & pl)
{
 paramlist_impl = new ParamListImpl();
 paramlist_impl->innermap = pl.paramlist_impl->innermap;
}

ParamList::~ParamList() { delete paramlist_impl; }

bool ParamList::Defined(const std::string & key) const 
{ 
 std::map<std::string, Parameter> & im = paramlist_impl->innermap;
 if (im.count(key) > 0) return true;
 return false;
}

ParamList & ParamList::operator=(const ParamList & pl)
{
 if (&pl != this) { paramlist_impl->innermap = pl.paramlist_impl->innermap; }
 return (*this);
}

Parameter & ParamList::operator[](const std::string & key) { return (paramlist_impl->innermap)[key]; }

const Parameter & ParamList::operator[](const std::string & key) const
{
 std::map<std::string, Parameter> & im = paramlist_impl->innermap;
 const std::map<std::string, Parameter>::const_iterator & p = im.find(key);
 return (*p).second;
}

void ParamList::Remove(const std::string & key) { (paramlist_impl->innermap).erase(key); }

Array<Parameter> ParamList::Parameters() const
{
 Array<Parameter> tmp;
 std::map<std::string, Parameter> & im = paramlist_impl->innermap;
 for (std::map<std::string, Parameter>::const_iterator it=im.begin();it != im.end();++it) tmp.Append(it->first);
 return tmp;
}

