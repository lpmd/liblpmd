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

   Parameter & operator[](const std::string & key);

   const Parameter & operator[](const std::string & key) const;   
 
   void Remove(const std::string & key);

   Array<Parameter> Parameters() const;

 private:
   class ParamListImpl * paramlist_impl;
};

} // lpmd

#endif


