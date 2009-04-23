//
//
//

#include "vector.h"
#include <iostream>
#include <cmath>
#include <cassert>

void test1()
{
 Vector v;
 for (int q=0;q<3;++q) assert(fabs(v[q]) < 1.0E-10);
}

void test2()
{
 Vector v(1.0, 2.0, 3.0);
 assert(fabs(v[0]-1.0) < 1.0E-10);
 assert(fabs(v[1]-2.0) < 1.0E-10);
 assert(fabs(v[2]-3.0) < 1.0E-10);
}

void test3()
{
 Vector v1("< 1.0, 2.0, 3.0 >");
 Vector v2("4.0 5.0 6.0");
 for (int q=0;q<3;++q) assert(fabs(v1[q]-(q+1.0)) < 1.0E-10);
 for (int q=0;q<3;++q) assert(fabs(v2[q]-(q+4.0)) < 1.0E-10);
}

void test4()
{
 double a[3] = { 1.0, 2.0, 3.0 };
 Vector v(a);
 for (int q=0;q<3;++q) assert(fabs(v[q]-(q+1)) < 1.0E-10);
}

void test5()
{
 assert(fabs(e1[0]-1.0) < 1.0E-10);
 assert((fabs(e1[1]) < 1.0E-10) && (fabs(e1[2]) < 1.0E-10));
 assert(fabs(e2[1]-1.0) < 1.0E-10);
 assert((fabs(e2[0]) < 1.0E-10) && (fabs(e2[2]) < 1.0E-10));
 assert(fabs(e3[2]-1.0) < 1.0E-10);
 assert((fabs(e3[0]) < 1.0E-10) && (fabs(e3[1]) < 1.0E-10));
}

void test6()
{
 Vector v;
 assert(fabs(v[0]) < 1.0E-10);
 v = e1;
 assert(fabs(v[0]-1.0) < 1.0E-10);
 Vector w(3.0, 4.0, 5.0);
 v = w;
 for (int q=0;q<3;++q) assert(fabs(v[q]-w[q]) < 1.0E-10);
}

void test7()
{
 Vector v = e1;
 v[1] = 3.0;
 v[2] = 7.0;
 assert(fabs(v[0]-1.0) < 1.0E-10); 
 assert(fabs(v[1]-3.0) < 1.0E-10);
 assert(fabs(v[2]-7.0) < 1.0E-10);
}

void test8()
{
 Vector v = RandomVector();
 assert(fabs(v.Module()-1.0) < 1.0E-10);
 Vector w = RandomVector(42.0);
 assert(fabs(w.Module()-42.0) < 1.0E-10);
}

void test9()
{
 Vector v1 = RandomVector();
 Vector v2 = RandomVector();
 Vector w1 = v1+v2;
 Vector w2 = v1-v2;
 for (int q=0;q<3;++q)
 {
  assert(fabs(w1[q]-v1[q]-v2[q]) < 1.0E-10);
  assert(fabs(w2[q]-v1[q]+v2[q]) < 1.0E-10);
 }
 Vector v(3.0, 4.0, 5.0);
 assert(fabs(Dot(v, e1)-3.0) < 1.0E-10);
 assert(fabs(Dot(v, e2)-4.0) < 1.0E-10);
 assert(fabs(Dot(v, e3)-5.0) < 1.0E-10);
 assert(fabs((Cross(e1, e2)-e3).Module()) < 1.0E-10);
 assert(fabs((Cross(e2, e3)-e1).Module()) < 1.0E-10);
 assert(fabs((Cross(e3, e1)-e2).Module()) < 1.0E-10);
}

void test10()
{
 Vector v(3.0, 4.0, 5.0);
 assert(fabs(v.Module()-sqrt(50.0)) < 1.0E-10);
 assert(fabs(v.SquareModule()-50.0) < 1.0E-10);

 assert(fabs(e1.Azimuth()-0.0) < 1.0E-10);
 assert(fabs(e2.Azimuth()-0.5*M_PI) < 1.0E-10);
 assert(fabs(e1.Zenith()-0.5*M_PI) < 1.0E-10);
 assert(fabs(e2.Zenith()-0.5*M_PI) < 1.0E-10);
 assert(fabs(e3.Zenith()) < 1.0E-10);

 for (int n=0;n<100;++n)
 {
  Vector w1 = RandomVector();
  double r = w1.Module();
  double phi = w1.Azimuth();
  double theta = w1.Zenith();
  Vector w2(r*cos(phi)*sin(theta), r*sin(phi)*sin(theta), r*cos(theta));
  assert(fabs((w2-w1).Module()) < 1.0E-10);
 }
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



