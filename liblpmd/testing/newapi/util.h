//
//
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>


//
//
//

void RemoveUnnecessarySpaces(std::string & input_string)
{
 // Remove multiple tabs
 for (std::string::size_type i = 0; ; )
 {	
  i = input_string.find_first_of('\t', i);	 
  if (i == std::string::npos) break;      
  ++i;	 
  while(isspace(input_string[i])) input_string.erase(i,1);	 
  ++i;	 
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
//
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
