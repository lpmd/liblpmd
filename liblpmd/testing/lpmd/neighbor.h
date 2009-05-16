/*
 *
 */

#ifndef __LPMD_NEIGHBOR_H__
#define __LPMD_NEIGHBOR_H__

#include <lpmd/vector.h>
#include <lpmd/atominterface.h>

using namespace lpmd;

namespace lpmd
{

 class Atom;               // forward declaration

 // 
 class Neighbor
 {
  public:
    AtomInterface * i;  //  
    AtomInterface * j;  //
    long int i_index;
    long int j_index;
    Vector rij;         // Distancia vectorial del atomo i al atomo j
    double r;           // Modulo de rij (precalculado para optimizacion)

  Neighbor(): i(0), j(0), rij(0.0, 0.0, 0.0), r(0.0) { }

  Neighbor(const Neighbor & nn) 
  {
   i = nn.i;
   j = nn.j;
   i_index = nn.i_index;
   j_index = nn.j_index;
   rij = nn.rij;
   r = nn.r;
  } 

  Neighbor & operator=(const Neighbor & nn)
  {
   if (&nn != this)
   {
    i = nn.i;
    j = nn.j;
    i_index = nn.i_index;
    j_index = nn.j_index;
    rij = nn.rij;
    r = nn.r;
   }
   return (*this);
  }

 };


}  // lpmd

#endif


