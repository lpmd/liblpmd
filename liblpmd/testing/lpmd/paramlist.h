//
//
//

#ifndef __LPMD_PARAMLIST_H__
#define __LPMD_PARAMLIST_H__

#include <string>
#include <list>

namespace lpmd
{

class ParamList
{
 public:
   //
   ParamList();
   virtual ~ParamList();

   bool Defined(const std::string & key) const;

   void AssignParameter(const std::string & key, std::string value);

   std::string GetString(const std::string & key);

   std::string GetString(const std::string & key) const;

   double GetDouble(const std::string & key);

   long GetInteger(const std::string & key);

   bool GetBool(const std::string & key);

   std::string & operator[](const std::string & key);
 
   void Remove(const std::string & key);

   std::list<std::string> Parameters() const;

 private:
   class ParamListImpl * paramlist_impl;
};

//
std::ostream & operator<<(std::ostream & os, const lpmd::ParamList & pl);

} // lpmd

#endif


