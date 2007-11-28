//
//
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include <lpmd/util.h>

//
//
//

void lpmd::EndWithError(std::string text)
{
 std::cerr << "[Error] " << text << '\n';
 exit(1);
}

bool lpmd::MustDo(long i, long start, long end, long step)
{
 if (i < start) return false;
 if (i > end) return false;
 if (i == start || i ==end) return true;
 if (((i - start) % step) == 0) return true;
 return false;
}

double lpmd::LeverRule(long i, long start, long end, double from_v, double to_v)
{
 if (start == end) return from_v;
 return (from_v + double(i-start)/double(end-start)*(to_v - from_v));
}

void lpmd::RemoveUnnecessarySpaces(std::string & input_string)
{
 // *** Remove spaces at start
 while (isspace(input_string[0])) input_string.erase(0,1);
 // *** Remove spaces from end
 while (isspace(input_string[input_string.length() -1]))
    input_string.erase(input_string.length() -1,1);
 // *** Remove multiple spaces
 for (std::string::size_type i = 0; ; )
 {	
  i = input_string.find_first_of(' ', i);
  if (i == std::string::npos) break; 
  ++i;
  while(isspace(input_string[i])) input_string.erase(i,1);	 
  ++i;	 
 }
 // *** Remove multiple tabs
 for (std::string::size_type i = 0; ; )
 {	
  i = input_string.find_first_of('\t', i);	 
  if (i == std::string::npos) break;      
  ++i;	 
  while(isspace(input_string[i])) input_string.erase(i,1);	 
  ++i;	 
 }
}

//
//
//
std::vector<std::string> lpmd::SplitTextLine(std::string line, char delimiter)
{
 RemoveUnnecessarySpaces(line);
 std::istringstream iss(line);
 std::vector<std::string> words;
 std::string l;
 while (std::getline(iss, l, delimiter)) words.push_back(l);
 return words;
}

std::list<std::string> lpmd::ListOfTokens(std::string line, char delimiter)
{
 RemoveUnnecessarySpaces(line);
 std::istringstream iss(line);
 std::list<std::string> words;
 std::string l;
 while (std::getline(iss, l, delimiter)) words.push_back(l);
 return words;
}

//
std::vector<std::string> lpmd::SplitSpeciesPair(std::string pair)
{
 return lpmd::SplitTextLine(pair, '-');
}


