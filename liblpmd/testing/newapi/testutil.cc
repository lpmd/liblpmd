//
//
//

#include "util.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>

void test1()
{
 std::string tmp = "     a";
 RemoveUnnecessarySpaces(tmp);
 assert (tmp=="a");
 tmp = "a      ";
 RemoveUnnecessarySpaces(tmp);
 assert (tmp=="a");
 tmp ="      a       b       c       ";
 RemoveUnnecessarySpaces(tmp);
 assert (tmp=="a b c");
}

void test2()
{
 std::string tmp="\ta";
 RemoveUnnecessarySpaces(tmp);
 assert (tmp=="a");
 tmp = "   a\t\t";
 RemoveUnnecessarySpaces(tmp);
 assert (tmp=="a");
 tmp = "\t   a\t\t   \tb \t    c\t  ";
 RemoveUnnecessarySpaces(tmp);
 std::cerr << tmp << '\n';
 assert (tmp=="a b c");
}

void test3()
{
 std::vector <std::string> vec =SplitTextLine("1 2 3 4 5 6 7 8 9 10",' ');
 for(int i=0 ; i< 10 ; ++i) assert(atof(vec[i].c_str())==i+1);
}

int main()
{
 test1();
 test2();
 test3();

 return 0;
}



