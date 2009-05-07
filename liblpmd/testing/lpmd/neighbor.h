/*
 *
 */

#ifndef __LPMD_NEIGHBOR_H__
#define __LPMD_NEIGHBOR_H__

#include <lpmd/vector.h>

using namespace lpmd;

namespace lpmd
{

 class Atom;               // forward declaration

 // 
 class Neighbor
 {
  public:
    Atom * i;    //  
    Atom * j;    //
    Vector rij;        // Distancia vectorial del atomo i al atomo j
    double r;          // Modulo de rij (precalculado para optimizacion)
 };

}  // lpmd

#endif


