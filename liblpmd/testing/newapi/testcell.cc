//
//
//

#include "cell.h"
#include <iostream>
#include <cmath>
#include <cassert>

void test1()
{
 Vector e[3];
 e[0]=Vector(1,0,0);e[1]=Vector(0,1,0);e[2]=Vector(0,0,1);
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
 Vector e1(1,0,0),e2(0,1,0),e3(0,0,1);
 assert((a[0]-e1).Module() < 1E-10);
 assert((a[1]-e2).Module() < 1E-10);
 assert((a[2]-e3).Module() < 1E-10);
}

void test4()
{
 Cell a("<1,2,3> <4 ,5,6> <7,8,9>");
 Cell b("1 2 3 4 5 6 7 8 9");
 Cell c("5 7 5 90 90 90");
 assert((a[0]-Vector(1,2,3)).Module()<1E-10); 
 assert((a[1]-Vector(4,5,6)).Module()<1E-10);
 assert((a[2]-Vector(7,8,9)).Module()<1E-10);
 assert((b[0]-Vector(1,2,3)).Module()<1E-10); 
 assert((b[1]-Vector(4,5,6)).Module()<1E-10);
 assert((b[2]-Vector(7,8,9)).Module()<1E-10);
 assert((c[0]-Vector(5,0,0)).Module()<1E-10);
 assert((c[1]-Vector(0,7,0)).Module()<1E-10);
 assert((c[2]-Vector(0,0,5)).Module()<1E-10);
}

void test5()
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

void test6()
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



