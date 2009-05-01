//
//
//
#include <lpmd/util.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include "config.h"

//
//
//

std::string lpmd::LibraryVersion() 
{
 return VERSION;
}

void lpmd::EndWithError(const std::string & text)
{
 std::cerr << "[Error] " << text << '\n';
 exit(1);
}

void lpmd::ShowWarning(const std::string who, const std::string text)
{
 std::cerr << "[Warning (from " << who << "): " << text << '\n';
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
 // Remove multiple tabs
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
////
////
////Ojo FindBetween no es const porque el string es modificado
std::vector<std::string> lpmd::FindBetween(std::string &line)
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

