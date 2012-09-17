/*
 *
 *
 *
 */

#include <lpmd/array.h>
#include <lpmd/timer.h>
#include <vector>
#include <iostream>

using namespace lpmd;

int main()
{
 const long int NTESTS = 500000;
 std::vector<int> sv;
 Array<int> a;

 Timer t;
 std::cout << "*** Testing push_back on std::vector\n";
 t.Start();
 for (long int n=0;n<NTESTS;++n)
 {
  sv.clear();
  for (int q=0;q<1000;++q)
  {
   sv.push_back(42);
  }
 }
 t.Stop();
 t.ShowElapsedTimes();
  
 std::cout << "\n*** Testing Append on lpmd::Array\n";
 t.Start();
 for (long int n=0;n<NTESTS;++n)
 {
  a.Clear();
  for (int q=0;q<1000;++q)
  {
   a.Append(42); 
  }
 }
 t.Stop();
 t.ShowElapsedTimes();

 std::cout << "\n*** Testing operator[] on std::vector\n";
 sv = std::vector<int>(5000);
 t.Start();
 for (long int n=0;n<NTESTS;++n)
 {
  double suma = 0.0;
  for (int q=0;q<5000;++q)
  {
   sv[q] = q;
   suma += double(sv[q]);
  }
 }
 t.Stop();
 t.ShowElapsedTimes();
  
 std::cout << "\n*** Testing operator[] on lpmd::Array\n";
 a = Array<int>(5000);
 t.Start();
 for (long int n=0;n<NTESTS;++n)
 {
  double suma = 0.0;
  for (int q=0;q<5000;++q)
  {
   a[q] = q;
   suma += double(a[q]);
  }
 }
 t.Stop();
 t.ShowElapsedTimes();

 return 0;
}

