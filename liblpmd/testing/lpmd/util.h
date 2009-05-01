//
//
//
#ifndef __LPMD_UTIL_H__
#define __LPMD_UTIL_H__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>

namespace lpmd
{

inline void EndWithError(const std::string & text)
{
 std::cerr << "[Error] " << text << '\n';
 exit(1);
}

inline void ShowWarning(const std::string who, const std::string text)
{
 std::cerr << "[Warning (from " << who << "): " << text << '\n';
}

//
//Convertidor de Grados a Radianes.
//
inline double DegreesToRadians(double d) { return (M_PI*d/180.0); }

//
//Remueve espacios innecesarios de un string
//
inline void RemoveUnnecessarySpaces(std::string & input_string)
{
 // Replace tabs by spaces.
 for (std::string::size_type i = 0; ; )
 {	
  i = input_string.find_first_of('\t', i); 
  if (i == std::string::npos) break;      
  else 
  {
   input_string.replace(i,1," ");	 
  }
 }
 // Remove spaces at start
 while (isspace(input_string[0])) input_string.erase(0,1);
 // Remove spaces from end
 while (isspace(input_string[input_string.length() -1]))
    input_string.erase(input_string.length() -1,1);
 // Remove multiple spaces
 for (std::string::size_type i = 0; ; )
 {	
  i = input_string.find_first_of(' ', i);
  if (i == std::string::npos) break; 
  ++i;
  while(isspace(input_string[i])) input_string.erase(i,1);	 
  ++i;	 
 }
}

//
// Devuelve una coleccion de strings desde un string separando por un delimitador
//
template <class T> T StringSplit(const std::string & line, char delimiter=' ')
{
 std::string tmpline(line);
 RemoveUnnecessarySpaces(tmpline);
 T words;
 std::string l;
 // Para manejar correctamente espacios y tabs
 if (delimiter == ' ')
 {
  std::string::size_type sp = std::string::npos;
  for (std::string::size_type p=0;p<tmpline.size();p++)
  {
   if (! isspace(tmpline[p])) 
   {
    if (sp == std::string::npos) sp = p;
   }
   else
   {
    if (sp != std::string::npos)
    {
     words.push_back(tmpline.substr(sp, p-sp));
     sp = std::string::npos;
    }
   }
  }
  if (sp != std::string::npos) words.push_back(tmpline.substr(sp, tmpline.size()-sp));
 }
 else 
 {
  std::istringstream iss(tmpline);
  while (std::getline(iss, l, delimiter)) words.push_back(l);
 }
 return words;
}

//
inline std::vector<std::string> SplitSpeciesPair(const std::string & pair)
{
 return StringSplit< std::vector<std::string> >(pair, '-');
}

template <class T> const std::string ToString(const T & object)
{
 std::ostringstream os;
 os << std::boolalpha << object;
 return(os.str());
}

inline std::vector<std::string> FindBetween(std::string & line)
{
 std::string tmpline(line);
 RemoveUnnecessarySpaces(tmpline);
 std::vector<std::string> words;
 size_t pc=0;
 size_t uc=0;
 int counter=0;
 while(1) 
 { 
  pc=tmpline.find_first_of("\"",uc+1);
  if(pc==std::string::npos) break;
  uc=tmpline.find_first_of("\"",pc+1);
  std::string t1=tmpline.substr(pc,uc-pc+1);
  t1.erase(0,1);
  t1.erase(t1.size()-1,1);
  words.push_back(t1);
  std::stringstream sst;
  sst << counter ;
  std::string t2="%"+sst.str();
  line.replace(pc,uc-pc+1,t2);
  counter++;
 }
 return words;
}

}

#endif

