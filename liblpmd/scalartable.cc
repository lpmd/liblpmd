//
//
//

#include <lpmd/scalartable.h>

using namespace lpmd;

ScalarTable::ScalarTable() { ClearAverage(); }

ScalarTable::~ScalarTable() { }

void ScalarTable::ClearAverage() 
{ 
 v_av = Matrix(); 
 n = 0;
}

void ScalarTable::AddToAverage() 
{
 if ((v_av.Columns() == 0) && (v_av.Rows() == 0)) v_av = Value();
 else v_av += Value(); 
 ++n;
}

void ScalarTable::OutputTo(std::ostream & os) const 
{
 os << Value();
}

void ScalarTable::OutputAverageTo(std::ostream & os) const
{
 if (n > 0) os << (v_av * (1.0 / double(n)));
 else os << 0.0e0;
}

