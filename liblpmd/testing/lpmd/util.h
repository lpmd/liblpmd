//
//
//
#ifndef __LPMD_UTIL_H__
#define __LPMD_UTIL_H__

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <cstdlib>

#include <lpmd/array.h>

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

inline double DegreesToRadians(double d) { return (M_PI*d/180.0); }

Array<std::string> ParseThreeVectors(const std::string & str);

long int SimpleHash(const std::string & text);

inline bool IsSpace(char x)
{
 return (((x == ' ') || (x == '\n')) || ((x == '\t') || (x == '\r')));
}

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
 while (IsSpace(input_string[0])) input_string.erase(0,1);
 // Remove spaces from end
 while (IsSpace(input_string[input_string.length() -1]))
    input_string.erase(input_string.length() -1,1);
 // Remove multiple spaces
 for (std::string::size_type i = 0; ; )
 {	
  i = input_string.find_first_of(' ', i);
  if (i == std::string::npos) break; 
  ++i;
  while(IsSpace(input_string[i])) input_string.erase(i,1);	 
  ++i;	 
 }
}

//
// Devuelve una coleccion de strings desde un string separando por un delimitador
//
Array<std::string> StringSplit(const std::string & line, char delimiter=' ');

//
inline Array<std::string> SplitSpeciesPair(const std::string & pair)
{
 return StringSplit(pair, '-');
}

template <class T> const std::string ToString(const T & object)
{
 std::ostringstream os;
 os << std::boolalpha << object;
 return(os.str());
}

inline Array<std::string> FindBetween(std::string & line)
{
 std::string tmpline(line);
 RemoveUnnecessarySpaces(tmpline);
 Array<std::string> words;
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
  words.Append(t1);
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

