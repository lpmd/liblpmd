//
//
//

#include <lpmd/containable.h>

#include <iostream>

using namespace lpmd;

IContainable::IContainable() { }

IContainable::~IContainable() { }

std::ostream & lpmd::operator<<(std::ostream & os, const IContainable & c) 
{ 
 c.OutputAverageTo(os); 
 return os;
} 
