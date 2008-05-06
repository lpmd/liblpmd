//
//
//

#include <lpmd/scalarvalue.h>

using namespace lpmd;

ScalarValue::ScalarValue() { ClearAverage(); }

ScalarValue::~ScalarValue() { }

void ScalarValue::ClearAverage() 
{ 
 v_av = 0.0e0; 
 n = 0;
}

void ScalarValue::AddToAverage() 
{
 v_av += Value(); 
 ++n;
}

void ScalarValue::OutputTo(std::ostream & os) const 
{
 os << Value();
}

void ScalarValue::OutputAverageTo(std::ostream & os) const
{
 if (n > 0) os << (v_av / double(n));
 else os << 0.0e0;
}

