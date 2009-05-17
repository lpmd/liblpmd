//
//
//
#include <lpmd/util.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

#include <lpmd/util.h>

using namespace lpmd;

Array<std::string> lpmd::StringSplit(const std::string & line, char delimiter)
{
 std::string tmpline(line);
 RemoveUnnecessarySpaces(tmpline);
 Array<std::string> words;
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
     words.Append(tmpline.substr(sp, p-sp));
     sp = std::string::npos;
    }
   }
  }
  if (sp != std::string::npos) words.Append(tmpline.substr(sp, tmpline.size()-sp));
 }
 else 
 {
  std::istringstream iss(tmpline);
  while (std::getline(iss, l, delimiter)) words.Append(l);
 }
 return words;
}

