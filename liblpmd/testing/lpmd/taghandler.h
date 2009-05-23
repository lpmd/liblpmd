/*
 *
 *
 *
 */

#ifndef __LPMD_TAGS_H__
#define __LPMD_TAGS_H__

#include <string>
#include <map>

#include <lpmd/error.h>
#include <lpmd/paramlist.h>

namespace lpmd
{

 typedef const std::string Tag;

 template <typename T> class TagHandler
 {
  public:
    ~TagHandler() 
    { 
     for (typename std::map<Tag, ParamList *>::const_iterator it=namedcontainers.begin();it!=namedcontainers.end();++it)
     { delete (it->second); }
    }

    void AssignTagGroup(const T & obj, Tag & container_tag)
    {
     if (namedcontainers.count(container_tag) == 0)
     {
      namedcontainers[container_tag] = new ParamList();
     }
     objmapper[&obj] = namedcontainers[container_tag]; 
    }

    Map & GetTagGroup(Tag & container_tag) 
    { 
     if (namedcontainers.count(container_tag) == 0) throw InvalidRequest("Tag "+container_tag);
     return *(namedcontainers[container_tag]); 
    }

    bool Have(const T & obj, Tag & tag)
    { 
     if (objmapper.count(&obj) == 0) return false;
     try 
     { 
      const ParamList & pl = *(objmapper[&obj]);
      return pl.Defined(tag);
     } 
     catch (std::exception & e) { return false; }
    }

    bool Have(const T & obj, Tag & tag) const
    { 
     if (objmapper.count(&obj) == 0) return false;
     try 
     { 
      const typename std::map<const T *, ParamList *>::const_iterator & p = objmapper.find(&obj);
      const ParamList & pl = *((*p).second);
      return pl.Defined(tag);
     } 
     catch (std::exception & e) { return false; }
    }

    bool HaveAny(Tag & tag)
    {
     for (typename std::map<Tag, ParamList *>::const_iterator it=namedcontainers.begin();it!=namedcontainers.end();++it)
     { 
      if (it->second->Defined(tag)) return true;
     }
     return false;
    }

    template <typename Z> void SetTag(const T & obj, Tag & tag, const Z & value)
    {
     if (objmapper.count(&obj) == 0) 
     {
      ParamList * pl = new ParamList();
      objmapper[&obj] = pl;
      namedcontainers[ToString<long int>(long(&obj))] = pl;
     }
     (*objmapper[&obj])[tag] = ToString<Z>(value);
    }

    const std::string GetTag(const T & obj, Tag & tag)
    {
     if (objmapper.count(&obj) == 0) throw InvalidRequest("Tag "+tag);
     try { return objmapper[&obj]->operator[](tag); }
     catch (std::exception & e) { throw InvalidRequest("Tag "+tag); }
    }

  private:
    std::map<Tag, ParamList *> namedcontainers;
    std::map<const T *, ParamList *> objmapper;
 };

}  // lpmd

#endif

