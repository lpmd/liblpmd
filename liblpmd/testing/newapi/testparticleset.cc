//
//
//

#include <cassert>
#include "particleset.h"
#include <lpmd/vector.h>

using namespace lpmd;

void test1()
{
 ParticleSet ps;
 assert(ps.Length() == 0);
}

void test2()
{
 ParticleSet ps(100);
 assert(ps.Length() == 100);
}

void test3()
{
 ParticleSet ps(50);
 const Vector pos(1.0, 2.0, 3.0);
 for (long int i=0;i<ps.Length();++i) ps[i].Position() = pos;
 for (long int i=0;i<ps.Length();++i) assert((ps[i].Position()-pos).Module() < 1.0E-10);
}

int main()
{
 test1();
 test2();
 test3();

 return 0;
}



