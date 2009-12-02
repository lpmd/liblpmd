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
    TagHandler<T> & operator=(const TagHandler<T> & orig)
    {
     if (&orig != this)
     {
      for (typename std::map<Tag, ParamList *>::const_iterator it=orig.namedcontainers.begin();it!=orig.namedcontainers.end();++it)
      {
       namedcontainers[it->first] = new ParamList();
       *(namedcontainers[it->first]) = *(it->second);
      }
      for (typename std::map<unsigned long int, ParamList *>::iterator it=orig.objmapper.begin();it!=orig.objmapper.end();++it)
      {
       objmapper[it->first] = new ParamList();
       *(objmapper[it->first]) = *(it->second);
      }
      for (typename std::map<Tag, int>::const_iterator it=orig.tagregistry.begin();it!=orig.tagregistry.end();++it)
          tagregistry[it->first] = 1;
      for (typename std::map<unsigned long int, int>::const_iterator it=orig.inside_named.begin();it!=orig.inside_named.end();++it)
          inside_named[it->first] = 1;
     }
     return *this;
    }

    ~TagHandler() 
    { 
     for (typename std::map<Tag, ParamList *>::const_iterator it=namedcontainers.begin();it!=namedcontainers.end();++it)
         delete (it->second);
     for (typename std::map<unsigned long int, ParamList *>::const_iterator it=objmapper.begin();it!=objmapper.end();++it)
     {
      if (inside_named.count(it->first) == 0) delete (it->second);
     }
    }

    void AssignTagGroup(const T & obj, Tag & container_tag)
    {
     if (namedcontainers.count(container_tag) == 0)
     {
      namedcontainers[container_tag] = new ParamList();
     }
     objmapper[obj.ID()] = namedcontainers[container_tag]; 
     inside_named[obj.ID()] = 1;
    }

    Map & GetTagGroup(Tag & container_tag) 
    { 
     if (namedcontainers.count(container_tag) == 0) throw InvalidRequest("Tag "+container_tag);
     return *(namedcontainers[container_tag]); 
    }

    bool Have(const T & obj, Tag & tag)
    { 
     if (objmapper.count(obj.ID()) == 0) return false;
     try 
     { 
      const ParamList & pl = *(objmapper[obj.ID()]);
      return pl.Defined(tag);
     } 
     catch (std::exception & e) { return false; }
    }

    bool Have(const T & obj, Tag & tag) const
    { 
     if (objmapper.count(obj.ID()) == 0) return false;
     try 
     { 
      const typename std::map<unsigned long int, ParamList *>::const_iterator & p = objmapper.find(obj.ID());
      const ParamList & pl = *((*p).second);
      return pl.Defined(tag);
     } 
     catch (std::exception & e) { return false; }
    }

    bool HaveAny(Tag & tag) 
    { 
     if (tagregistry.count(tag) > 0) return true; 
     for (typename std::map<Tag, ParamList *>::const_iterator it=namedcontainers.begin();it!=namedcontainers.end();++it)
          if ((it->second)->Defined(tag)) return true;
     return false;
    }

    void RemoveTags(const T & obj)
    {
     if (objmapper.count(obj.ID()) != 0)
     {
      std::string todelete = "";
      for (typename std::map<Tag, ParamList *>::const_iterator it=namedcontainers.begin();it!=namedcontainers.end();++it)
      {
       if (it->second == objmapper[obj.ID()]) { todelete = it->first; break; }
      }
      if (todelete != "") namedcontainers.erase(todelete);
      delete objmapper[obj.ID()];
      objmapper.erase(obj.ID());
     }
    }

    void SetTag(const T & obj, Tag & tag, const std::string & value)
    {
     if (objmapper.count(obj.ID()) == 0) 
     {
      ParamList * pl = new ParamList();
      objmapper[obj.ID()] = pl;
     }
     (*objmapper[obj.ID()])[tag] = value;
     tagregistry[tag] = 1;
    }

    template <typename Z> void SetTag(const T & obj, Tag & tag, const Z & value)
    {
     if (objmapper.count(obj.ID()) == 0) 
     {
      ParamList * pl = new ParamList();
      objmapper[obj.ID()] = pl;
     }
     (*objmapper[obj.ID()])[tag] = ToString<Z>(value);
     tagregistry[tag] = 1;
    }

    const std::string GetTag(const T & obj, Tag & tag)
    {
     if (objmapper.count(obj.ID()) == 0) throw InvalidRequest("Tag "+tag);
     try { return objmapper[obj.ID()]->operator[](tag); }
     catch (std::exception & e) { throw InvalidRequest("Tag "+tag); }
    }

    const std::string GetTag(const T & obj, Tag & tag) const
    {
     if (objmapper.count(obj.ID()) == 0) throw InvalidRequest("Tag "+tag);
     try 
     {
      const typename std::map<unsigned long int, ParamList *>::const_iterator & p = objmapper.find(obj.ID());
      return ((*p).second)->operator[](tag);
     }
     catch (std::exception & e) { throw InvalidRequest("Tag "+tag); }
    }

    Array<Parameter> GetTags(const T & obj) const 
    {
     Array<Parameter> tmp;
     if (objmapper.count(obj.ID()) == 0) return tmp; 
     return objmapper[obj.ID()]->Parameters();
    }

  private:
    std::map<Tag, ParamList *> namedcontainers;
    mutable std::map<unsigned long int, ParamList *> objmapper;
    mutable std::map<Tag, int> tagregistry;
    mutable std::map<unsigned long int, int> inside_named;
 };

}  // lpmd

#endif

