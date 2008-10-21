
#include <lpunit/testsuite.h>
#include <iostream>
#include <vector>

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

void TestSuite::SetTestsSetup(void (*setupfunc)())
{
 impl->setupfunc = setupfunc; 
}

void TestSuite::SetTestsTeardown(void (*tdfunc)())
{
 impl->tdfunc = tdfunc; 
}

void TestSuite::Register(Test * newtest)
{
 impl->tests.push_back(newtest);
}

int TestSuite::PerformAllTests()
{
 unsigned long int n = impl->tests.size();
 std::cout << impl->description << " (" << n << " tests) : " << '\n';
 int cfailed = 0;
 for (unsigned long int q=0;q<n;++q)
 {
  std::cout << "   -> " << impl->tests[q]->Description() << ": ";
  std::cout.flush();
  if (impl->setupfunc != NULL) 
     (*(impl->setupfunc))();            // calls the setup function for the suite
  bool r = impl->tests[q]->Check();
  if (impl->tdfunc != NULL) 
     (*(impl->tdfunc))();               // calls the teardown function for the suite
  if (r == true) std::cout << "OK\n";
  else
  {
   std::cout << "FAILED\n";
   cfailed++;
  }
 }
 std::cout << "\n-> " << impl->description << ": ";
 if (cfailed > 0)
 {
  std::cout << "*** " << cfailed << " of " << n << " tests (" << 100.0*float(cfailed)/float(n) << "%) failed ***" << '\n';
  return 1; // some test(s) failed
 }
 else 
 {
  std::cout << "!!! All " << n << " tests passed !!!" << '\n';
  return 0;  // all tests OK!
 }
}


