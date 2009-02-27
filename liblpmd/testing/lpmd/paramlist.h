//
//
//

#ifndef __LPMD_PARAMLIST_H__
#define __LPMD_PARAMLIST_H__

#include <lpmd/map.h>

namespace lpmd
{

class ParamList: public Map
{
 public:
   //
   ParamList();
   virtual ~ParamList();

   bool Defined(const std::string & key) const;

   virtual void AssignParameter(const std::string & key, std::string value);

   std::string & operator[](const std::string & key);

   const std::string & operator[](const std::string & key) const;   
 
   void Remove(const std::string & key);

   std::list<std::string> Parameters() const;

 private:
   class ParamListImpl * paramlist_impl;
};

} // lpmd

#endif


