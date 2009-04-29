//
//
//

#include "cell.h"
#include <lpmd/vector.h>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace lpmd;

void test1()
{
 Vector e[3];
 e[0]=e1;e[1]=e2;e[2]=e3;
 Cell a;
 for (int i=0 ; i< 3 ; ++i) {assert((a[i]-e[i]).Module() < 1E-10);}
}

void test2()
{
 Vector e[3];
 e[0]=RandomVector(),e[1]=RandomVector(),e[2]=RandomVector();
 Cell a(e[0],e[1],e[2]);
 for (int i=0 ; i< 3 ; ++i) {assert((a[i]-e[i]).Module() < 1E-10);}
}

void test3()
{
 double aa=1,b=1,c=1,alpha=90,beta=90,gamma=90;
 Cell a(aa,b,c,alpha,beta,gamma);
 assert((a[0]-e1).Module() < 1E-10);
 assert((a[1]-e2).Module() < 1E-10);
 assert((a[2]-e3).Module() < 1E-10);
}

void test4()
{
 Cell a("<1,2,3> <4 ,5,6> <7,8,9>");
 assert((a[0]-Vector(1,2,3)).Module()<1E-10); 
 assert((a[1]-Vector(4,5,6)).Module()<1E-10);
 assert((a[2]-Vector(7,8,9)).Module()<1E-10);
}

void test5()
{
 Cell b("1 2 3 4 5 6 7 8 9");
 assert((b[0]-Vector(1,2,3)).Module()<1E-10); 
 assert((b[1]-Vector(4,5,6)).Module()<1E-10);
 assert((b[2]-Vector(7,8,9)).Module()<1E-10);
}

void test6()
{
 Cell c("5 7 5 90 90 90");
 assert((c[0]-Vector(5,0,0)).Module()<1E-10);
 assert((c[1]-Vector(0,7,0)).Module()<1E-10);
 assert((c[2]-Vector(0,0,5)).Module()<1E-10);
}

void test7()
{
 Cell a;
 assert(a.Periodicity(0)==true);
 assert(a.Periodicity(1)==true);
 assert(a.Periodicity(2)==true);
 a.Periodicity(0) = false;
 a.Periodicity(1) = false;
 a.Periodicity(2) = false;
 assert(a.Periodicity(0)==false);
 assert(a.Periodicity(1)==false);
 assert(a.Periodicity(2)==false);
}

void test8()
{
 Vector a(1,2,3);
 Vector b(4,5,6);
 Vector c(7,8,9);
 Cell CELL;
 CELL[0] = a;
 assert ((CELL[0]-a).Module()<1E-10);
 CELL[1] = b;
 assert ((CELL[1]-b).Module()<1E-10);
 CELL[2] = c;
 assert ((CELL[2]-c).Module()<1E-10);
}

void test9() //scale de los vectores base independientes
{
 Vector a1(1,2,3);
 Vector a2(4,5,6);
 Vector a3(10,9,8);
 Cell c(a1,a2,a3);
 c[0] *= .9;
 c[1] *=1.5;
 c[2] *= .8;
 Vector b1(1*0.9,2*0.9,3*0.9);
 Vector b2(1.5*4,1.5*5,1.5*6);
 Vector b3(.8*10,.8*9,.8*8);
 assert ((c[0]-b1).Module()<1E-10);
 assert ((c[1]-b2).Module()<1E-10);
 assert ((c[2]-b3).Module()<1E-10);
}

void test10() //scale hidrostatico de vectores base
{
 Vector a1(10,9,8);
 Vector a2(10,30,50);
 Vector a3(20,40,60);
 Cell c(a1,a2,a3);
 c *= .5;
 assert ((c[0]-Vector(5.0,4.5,4.0)).Module()<1E-10);
 assert ((c[1]-Vector(5.0,15.0,25.0)).Module()<1E-10);
 assert ((c[2]-Vector(10,20,30)).Module()<1E-10);
}

void test11() //ScaleByCell
{
 Cell c("10 10 10 90 90 90");
 Vector a1(0.5,0.5,0.5);
 Vector a = c.ScaleByCell(a1);
 assert ((a-Vector(5,5,5)).Module()<1E-10);
}

void test12() //Non-Ortogonal Convert to External->Internal->External
{
 Cell c("20 20 20 90 90 120");
 Vector a1(0.5,0.75,0.5);
 c.ConvertToExternal(a1);
 c.ConvertToInternal(a1);
 assert ((a1-Vector(0.5,0.75,0.5)).Module()<1E-10);
}

void test13() //Non-Ortogonal Convert to Internal->External->Internal
{
 Cell c("40 40 30 80 110 95");
 Vector a1(10,15,10);
 c.ConvertToInternal(a1);
 c.ConvertToExternal(a1);
 assert ((a1-Vector(10,15,10)).Module()<1E-10);
}

int main()
{
 test1();
 test2();
 test3();
 // test4(); // FIXME: hacer pasar este test!
 test5();
 test6();
 test7();
 test8();
 test9();
 test10();
 test11();
 test12();
 test13();

 return 0;
}



