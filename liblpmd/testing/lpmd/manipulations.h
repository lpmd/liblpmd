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

/*
inline Vector UndoPeriodicity(BasicParticleSet & part, BasicCell & cell)
{
 SimulationCell scratch(simcell[0]);
 Vector ** noperiodic = new Vector*[N];
 for (int t=0;t<N;++t) noperiodic[t] = new Vector[nat];
 for (int i=0;i<nat;++i) noperiodic[0][i] = simcell[0].GetAtom(i).Position();

 for (int t=1;t<N;++t)
  for (int i=0;i<nat;++i)
  {
   scratch.SetPosition(0, simcell[t-1].GetAtom(i).Position());
   scratch.SetPosition(1, simcell[t].GetAtom(i).Position());
   noperiodic[t][i] = noperiodic[t-1][i] + scratch.VectorDistance(0, 1);
  }
}
*/

}

#endif

