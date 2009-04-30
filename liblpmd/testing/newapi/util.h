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


//
//Convertidor de Grados a Radianes.
//

double DegreesToRadians(double d) { return (M_PI*d/180.0); }

//
//Remueve espacios innecesarios de un string
//

void RemoveUnnecessarySpaces(std::string & input_string)
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
// Devuelve un vector de strings de un std::string separando por un caracter.
//

std::vector<std::string> SplitTextLine(const std::string & line, char delimiter)
{
 std::string tmpline(line);
 RemoveUnnecessarySpaces(tmpline);
 std::istringstream iss(tmpline);
 std::vector<std::string> words;
 std::string l;
 while (std::getline(iss, l, delimiter)) words.push_back(l);
 return words;
}

#endif
