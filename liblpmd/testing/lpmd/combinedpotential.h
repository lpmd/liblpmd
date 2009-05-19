/*
 *
 *
 *
 */

#ifndef __LPMD_COMBINED_POTENTIAL_H__
#define __LPMD_COMBINED_POTENTIAL_H__

#include <lpmd/potential.h>
#include <lpmd/array.h>

namespace lpmd
{

class CombinedPotential: public Array<Potential &>, public Potential
{
 public:
   double energy(Configuration & conf)
   {
    double en = 0.0;
    for (int p=0;p<Size();++p) en += (*this)[p].energy(conf);
    return en;
   }

   void UpdateForces(Configuration & conf)
   {
    conf.CellManager().UpdateCell(conf);
    for (int p=0;p<Size();++p) (*this)[p].UpdateForces(conf);
   }
};

}

#endif

