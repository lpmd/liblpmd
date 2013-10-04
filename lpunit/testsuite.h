/*
 *
 *
 *
 */

#ifndef __LPUNIT_TESTSUITE_H__
#define __LPUNIT_TESTSUITE_H__

#include <lpunit/test.h>

namespace lpunit
{

class TestSuite
{
 public:
   //
   TestSuite(std::string desc);
   virtual ~TestSuite();

   //
   void SetTestsSetup(void (*setupfunc)());
   void SetTestsTeardown(void (*tdfunc)());
   void Register(Test * newtest);
   int PerformAllTests();

 private:
   class TestSuiteImpl * impl;
};

}

#endif

