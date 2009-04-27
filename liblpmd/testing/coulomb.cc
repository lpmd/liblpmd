//
//
//

#include <lpmd/coulomb.h>

#include <iostream>

using namespace lpmd;

Coulomb::Coulomb(double Q1, double Q2)
{
 q1 = Q1;
 q2 = Q2;
}

Coulomb::Coulomb()
{
 q1 = 0;
 q2 = 0;
}

Coulomb::~Coulomb() { }

void Coulomb::SetValue1(double Q1)
{
 q1 = Q1;
}

void Coulomb::SetValue2(double Q2)
{
 q2 = Q2;
}

double Coulomb::pairEnergy(const double & r) const
{
 double E = q1*q2/r;
 return E; 
}

Vector Coulomb::pairForce(const Vector & r) const
{
 double rr = r.Module();
 double ff = q1*q2/(rr*rr*rr);
 Vector fv = r*ff;
// fv.Scale(ff);
 return fv;
}

