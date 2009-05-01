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

void test7()
{
 std::vector<std::string> sp;
 sp = SplitSpeciesPair("Ar-Kr");
 assert((sp[0] == "Ar") && (sp[1] == "Kr"));
 sp = SplitSpeciesPair("N-N");
 assert((sp[0] == "N") && (sp[1] == "N"));
}

void test8()
{
 int start=5, end=210, each=50;
 std::string accum = "";
 for (int i=0;i<250;++i) 
 {
  if (MustDo(i, start, end, each)) accum += (ToString<int>(i)+" ");
 }
 assert(accum == "5 55 105 155 205 ");
 // ojo: posible bug en la implementacion original de MustDo! incluia el punto end siempre
}

void test9()
{
 std::string line = "hola mundo \"esta es una\" prueba";
 std::vector<std::string> vec = FindBetween(line);
 assert(line == "hola mundo %0 prueba");
 assert(vec.size() == 1);
 assert(vec[0] == "esta es una");
 // que pasa con mas de un par de comillas?
}

void test10()
{
 double initial = 0.0, final = 100.0;
 int start = 50, end = 1050;
 assert(fabs(LeverRule(start, start, end, initial, final)-initial) < 1.0E-10); 
 assert(fabs(LeverRule(end, start, end, initial, final)-final) < 1.0E-10); 
 assert(fabs(LeverRule(550, start, end, initial, final)-50.0) < 1.0E-10);
 assert(fabs(LeverRule(250, start, end, initial, final)-20.0) < 1.0E-10);
 assert(fabs(LeverRule(750, start, end, initial, final)-70.0) < 1.0E-10);
}

int main()
{
 test1();
 test2();
 test3();
 test4();
 test5();
 test6();
 test7();
 test8();
 test9();
 test10();

 return 0;
}

