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
//Convertidor de Grados a Radianes.
//
inline void UnCenter(BasicParticleSet & part, BasicCell & cell) 
{
 const long n = part.Size();
 Vector displacement = cell.ScaleByCell(Vector(0.5,0.5,0.5));
 for (long i = 0; i< n ; ++i)
 {
  part[i].Position() = part[i].Position() + displacement;
 }
}


}

#endif

