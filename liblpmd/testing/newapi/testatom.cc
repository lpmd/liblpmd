//
//
//

#include "atom.h"
#include "vector3.h"
#include <iostream>
#include <cmath>
#include <cassert>

void test1()
{
 Atom at("H");
 assert(at.Z() == 1);
 assert(at.Symbol() == "H");
 assert(at.Position().Module() < 1.0E-10);
 Atom at2(26);
 assert(at2.Z() == 26);
 assert(at2.Symbol() == "Fe");
}

void test2()
{
 Atom at("H");
 Vector3 pos(2.0, 3.0, 4.0);
 at.Position() = pos;
 assert((at.Position()-pos).Module() < 1.0E-10); 
 Vector3 vel(0.5, 0.2, 3.4);
 at.Velocity() = vel;
 assert((at.Velocity()-vel).Module() < 1.0E-10); 
 Vector3 acc(50.0, 70.0, 35.0);
 at.Acceleration() = acc;
 assert((at.Acceleration()-acc).Module() < 1.0E-10); 
}

void test3()
{
 Vector3 pos(2.0, 3.0, 4.0), vel(0.5, 0.2, 3.4), acc(50.0, 70.0, 35.0);
 Atom at("H", pos);
 assert(at.Z() == 1);
 assert(at.Symbol() == "H");
 assert((at.Position()-pos).Module() < 1.0E-10); 
 Atom at2("H", pos, vel);
 assert(at2.Z() == 1);
 assert(at2.Symbol() == "H");
 assert((at2.Position()-pos).Module() < 1.0E-10); 
 assert((at2.Velocity()-vel).Module() < 1.0E-10); 
 Atom at3("H", pos, vel, acc);
 assert(at3.Z() == 1);
 assert(at3.Symbol() == "H");
 assert((at3.Position()-pos).Module() < 1.0E-10); 
 assert((at3.Velocity()-vel).Module() < 1.0E-10); 
 assert((at3.Acceleration()-acc).Module() < 1.0E-10);
}

void test4()
{
 Atom at("Ar");
 assert(fabs(at.Mass()-39.948) < 1.0E-10); 
}

int main()
{
 test1();
 test2();
 test3();
 test4();

 return 0;
}



