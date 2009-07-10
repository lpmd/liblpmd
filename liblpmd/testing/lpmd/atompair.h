/*
 *
 */

#ifndef __LPMD_ATOMPAIR_H__
#define __LPMD_ATOMPAIR_H__

#include <lpmd/vector.h>
#include <lpmd/array.h>
#include <lpmd/basicatom.h>

namespace lpmd
{

 class AtomPair
 {
  public:
    BasicAtom * i;  //  
    BasicAtom * j;  //
    long int i_index;
    long int j_index;
    Vector rij;         // Distancia vectorial del atomo i al atomo j
    double r2;          // SquareModule() de rij.

  AtomPair(): i(0), j(0), rij(0.0, 0.0, 0.0), r2(0.0) { }

  AtomPair(const AtomPair & nn) 
  {
   i = nn.i;
   j = nn.j;
   i_index = nn.i_index;
   j_index = nn.j_index;
   rij = nn.rij;
   r2 = nn.r2;
  } 

  AtomPair & operator=(const AtomPair & nn)
  {
   if (&nn != this)
   {
    i = nn.i;
    j = nn.j;
    i_index = nn.i_index;
    j_index = nn.j_index;
    rij = nn.rij;
    r2 = nn.r2;
   }
   return (*this);
  }

 };

 typedef Array<AtomPair> NeighborList;

}  // lpmd

#endif

