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

 std::vector<std::string> SplitTextLine(const std::string & line, char delimiter=' ');
 
 std::list<std::string> ListOfTokens(const std::string & line, char delimiter=' ');

 std::vector<std::string> SplitSpeciesPair(const std::string & pair);

 template <class T> const std::string ToString(const T & object)
 {
  std::ostringstream os;
  os << object;
  return(os.str());
 }

} // lpmd

#endif

