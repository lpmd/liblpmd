//
//
//
#ifndef __LPMD_MANIPULATIONS_H__
#define __LPMD_MANIPULATIONS_H__

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <cstdlib>

#include <lpmd/array.h>
#include <lpmd/basicparticleset.h>
#include <lpmd/basiccell.h>
#include <lpmd/vector.h>

namespace lpmd
{

//
//
inline void UnCenter(BasicParticleSet & part, BasicCell & cell) 
{
 const long n = part.Size();
 Vector displacement = cell.ScaleByCell(Vector(0.5,0.5,0.5));
 for (long i=0;i<n;++i)
 {
  part[i].Position() += displacement;
 }
}

inline void CenterByCenterOfMass(BasicParticleSet & part, BasicCell & cell) 
{
 const long n = part.Size();
 Vector centerofmass = part.CenterOfMass();
 Vector center = cell.Cartesian(Vector(0.5, 0.5, 0.5));
 for (long i=0;i<n;++i) part[i].Position() -= (centerofmass-center);
}

}

#endif

