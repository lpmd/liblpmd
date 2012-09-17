/*
 *
 *
 *
 */

#include <lpunit/test.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace lpunit;

//
//
//
class lpunit::SimpleTestImpl
{
 public:
   std::string description;
   void (*testfunction)();
};

SimpleTest::SimpleTest(std::string desc, void (*testfunc)()): Test(desc)
{
 impl = new SimpleTestImpl();
 impl->testfunction = testfunc;
}

SimpleTest::~SimpleTest() { delete impl; }

bool SimpleTest::Check() 
{
 Setup(); 
 bool result = true;
 try
 {
  (*(impl->testfunction))(); 
  Teardown();
 }
 catch(TestFailed & tf) { result = false; }
 Teardown();
 return result;
}

