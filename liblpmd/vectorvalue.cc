//
//
//

#include <lpmd/vectorvalue.h>

using namespace lpmd;

VectorValue::VectorValue() { ClearAverage(); }

VectorValue::~VectorValue() { }

void VectorValue::ClearAverage()
{ 
 v_av = Vector(0.0e0); 
 n = 0;
}

void VectorValue::AddToAverage()
{
 v_av = v_av + Value(); 
 ++n;
}

void VectorValue::OutputTo(std::ostream & os) const
{
 os << Value();
}

void VectorValue::OutputAverageTo(std::ostream & os) const
{
 Vector av;
 if (n > 0) av = v_av / n;
 os << av;
}

