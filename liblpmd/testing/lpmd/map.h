//
//
//

#ifndef __LPMD_MAP_H__
#define __LPMD_MAP_H__

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <lpmd/vector.h>

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

   inline bool GetBool(const std::string & key)
   {
    if ((*this)[key] == "true") return true;
    else return false;
   }

   inline Vector GetVector(const std::string & key)
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


