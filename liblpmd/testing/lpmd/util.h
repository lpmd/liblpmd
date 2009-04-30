//
//
//

#ifndef __LPMD_UTIL_H__
#define __LPMD_UTIL_H__

#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <cstdlib>

namespace lpmd
{

 std::string LibraryVersion(); 

 void EndWithError(const std::string & text);

 void ShowWarning(const std::string who, const std::string text);

 bool MustDo(long i, long start, long end, long step);

 double LeverRule(long i, long start, long end, double from_v, double to_v);

 void RemoveUnnecessarySpaces(std::string & input_string);

 std::vector<std::string> FindBetween(std::string & line);
 
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
  os << object;
  return(os.str());
 }

} // lpmd

#endif

