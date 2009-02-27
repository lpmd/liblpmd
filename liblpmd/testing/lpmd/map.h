//
//
//

#ifndef __LPMD_MAP_H__
#define __LPMD_MAP_H__

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

namespace lpmd
{
 //

 class Map
 {
  public:
   //
   Map() { }
   virtual ~Map() { }

   virtual bool Defined(const std::string & key) const = 0;

   virtual void AssignParameter(const std::string & key, std::string value) = 0;
   
   virtual std::string & operator[](const std::string & key) = 0;
  
   virtual const std::string & operator[](const std::string & key) const = 0;

   virtual void Remove(const std::string & key) = 0;

   virtual std::list<std::string> Parameters() const = 0;

   // 
   //
   //
   inline std::string GetString(const std::string & key) { return (*this)[key]; }

   inline std::string GetString(const std::string & key) const { return (*this)[key]; }

   inline double GetDouble(const std::string & key) { return atof((*this)[key].c_str()); }

   inline long GetInteger(const std::string & key) { return atoi((*this)[key].c_str()); }

   bool GetBool(const std::string & key)
   {
    if ((*this)[key] == "true") return true;
    else return false;
   }

};

//
inline std::ostream & operator<<(std::ostream & os, const lpmd::Map & m)
{
 std::list<std::string> params = m.Parameters();
 for (std::list<std::string>::const_iterator it=params.begin();it != params.end();++it)
   os << (*it) << " = " << m[*it] << '\n';
 return os;
}

} // lpmd

#endif


