
#include <lpunit/testsuite.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>

using namespace lpunit;

//
//
//
class lpunit::TestSuiteImpl
{
 public:
   std::string description;
   std::vector<Test *> tests;
   void (*setupfunc)();
   void (*tdfunc)();
};

TestSuite::TestSuite(std::string desc) 
{ 
 impl = new TestSuiteImpl();
 impl->description = desc;
 impl->setupfunc = NULL;
 impl->tdfunc = NULL;
}

TestSuite::~TestSuite() 
{ 
 for (unsigned long int q=0;q<impl->tests.size();++q)
     delete impl->tests[q];
 delete impl; 
}

void TestSuite::SetTestsSetup(void (*setupfunc)()) { impl->setupfunc = setupfunc; }

void TestSuite::SetTestsTeardown(void (*tdfunc)()) { impl->tdfunc = tdfunc; }

void TestSuite::Register(Test * newtest) { impl->tests.push_back(newtest); }

int TestSuite::PerformAllTests()
{
 unsigned long int n = impl->tests.size();
 std::cout << '\n' << impl->description << " (" << n << " tests) : " << '\n';
 int cfailed = 0;
 if (n == 0)
 {
  printf("*** \e[31mNo tests defined\e[0m, this counts as a failure ***\n");
  return 1;
 }
 for (unsigned long int q=0;q<n;++q)
 {
  std::string title = impl->tests[q]->Description();
  std::cout << "   -> " << std::left << std::setw(65) << title << "  ";
  std::cout.flush();
  if (impl->setupfunc != NULL) (*(impl->setupfunc))(); // calls the setup function for the suite
  bool r;
  std::string errormsg="";
  try { r = impl->tests[q]->Check(); }
  catch (std::exception & e) 
  { 
   errormsg = ("[Fail] Exception raised: "+std::string(e.what())+"\n");
   r = false;
  }
  if (impl->tdfunc != NULL) (*(impl->tdfunc))(); // calls the teardown function for the suite
  if (r == true) printf("\e[32mOK\e[0m\n");
  else
  {
   if (errormsg != "") std::cout << errormsg << '\n';
   cfailed++;
  }
 }
 std::cout << "\n-> " << impl->description << ": ";
 if (cfailed > 0)
 {
  printf("*** %d of %d tests (%.3f %%) \e[31mFAILED\e[0m ***\n", cfailed, n, 100.0*float(cfailed)/n);
  return 1; // some test(s) failed
 }
 else 
 {
  printf("!!! All %d tests \e[32mPASSED\e[0m !!!\n", n);
  return 0;  // all tests OK!
 }
}


