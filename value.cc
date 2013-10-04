/*
 *
 *
 *
 */

#include <iostream>
#include <fstream>

#include <lpmd/value.h>

using namespace lpmd;

AbstractValue::~AbstractValue() { }

void AbstractValue::OutputToFile(const std::string & filename) const
{
 if (filename == "-") OutputTo(std::cout); 
 else
 {
  std::ofstream f(filename.c_str());
  OutputTo(f);
 }
}

