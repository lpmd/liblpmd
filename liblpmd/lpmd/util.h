//
//
//

#ifndef __LPMD_UTIL_H__
#define __LPMD_UTIL_H__

#include <string>
#include <list>
#include <vector>

namespace lpmd
{

void EndWithError(std::string text);

bool MustDo(long i, long start, long end, long step);

double LeverRule(long i, long start, long end, double from_v, double to_v);

void RemoveUnnecessarySpaces(std::string & input_string);

std::vector<std::string> SplitTextLine(std::string line, char delimiter=' ');
std::list<std::string> ListOfTokens(std::string line, char delimiter=' ');

std::vector<std::string> SplitSpeciesPair(std::string pair);

} // lpmd

#endif

