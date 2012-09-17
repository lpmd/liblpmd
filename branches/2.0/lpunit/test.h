/*
 *
 *
 *
 */

#ifndef __LPUNIT_TEST_H__
#define __LPUNIT_TEST_H__

#include <string>
#include <iostream>
#include <cstdarg>

namespace lpunit
{

 //
 class TestFailed
 {
  public:
    TestFailed() { }
 };

 //
 //
 //
 inline void Assert(bool result)
 {
  if (result == false) throw lpunit::TestFailed();
 }

 class Test
 {
  public:
   //
   Test(std::string desc): description(desc) { }
   virtual ~Test() { }

   const std::string & Description() const { return description; }

   virtual void Setup() { }
   virtual void Teardown() { }

   virtual bool Check() = 0;

  private:
   std::string description;
 };

class SimpleTest: public Test
{
 public:
   // 
   SimpleTest(std::string desc, void (*testfunc)());
   virtual ~SimpleTest();

   bool Check();

 private:
   class SimpleTestImpl * impl;
};


}

#endif

