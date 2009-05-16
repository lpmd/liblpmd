//
//
//

#include <lpmd/instantproperty.h>
#include <lpmd/util.h>

#include <iostream>

using namespace lpmd;

InstantProperty::InstantProperty() { }

InstantProperty::~InstantProperty() { }

void InstantProperty::Evaluate(Configuration & conf, Potential & pot)
{
 ShowWarning("instantproperty", "Ejecutando InstantProperty::Evaluate, que es una implementacion vacia!");
}
