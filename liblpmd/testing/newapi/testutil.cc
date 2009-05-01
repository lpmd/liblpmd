//
//
//

#include "util.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>
#include <list>

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
 assert (tmp=="a b c");
}

void test3()
{
 std::vector <std::string> vec = StringSplit< std::vector<std::string> >("1 2 3 4 5 6 7 8 9 10",' ');
 assert(vec.size() == 10);
 for(int i=0;i<10;++i) assert(int(atof(vec[i].c_str())) == i+1);
}

void test4()
{
 std::list <std::string> lst =StringSplit< std::list<std::string> >("1 2 3 4 5 6 7 8 9 10",' ');
 int i = 1;
 for (std::list<std::string>::const_iterator it=lst.begin();it!=lst.end();++it)
 {
  assert(int(atof((*it).c_str())) == i);
  i++;
 }
}

void test5()
{
 double g=180;
 double rad=DegreesToRadians(g);
 assert (rad == M_PI);
 g = 360;
 rad = DegreesToRadians(g);
 assert (rad == 2*M_PI);
 g = 0;
 rad = DegreesToRadians(g);
 assert (rad == 0);
 g = 45;
 rad = DegreesToRadians(g);
 assert (rad == M_PI/4);
}

void test6()
{
 assert(ToString<int>(42) == "42");
 assert(ToString<double>(3.14159) == "3.14159");
 assert(ToString<bool>(true) == "true");
 assert(ToString<bool>(false) == "false");
}

int main()
{
 test1();
 test2();
 test3();
 test4();
 test5();
 test6();

 return 0;
}

