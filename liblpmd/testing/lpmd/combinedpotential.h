/*
 *
 *
 *
 */

#ifndef __LPMD_COMBINED_POTENTIAL_H__
#define __LPMD_COMBINED_POTENTIAL_H__

#include <lpmd/potential.h>
#include <lpmd/array.h>
#include <lpmd/error.h>

namespace lpmd
{

class CombinedPotential: public Array<Potential &>, public Potential
{
 public:

   void Initialize(Configuration & conf)
   {
    Potential::Initialize(conf);
    for (int p=0;p<Size();++p) (*this)[p].Initialize(conf);
   }

   double energy(Configuration & conf)
   {
    double en = 0.0;
    for (int p=0;p<Size();++p) en += (*this)[p].energy(conf);
    return en;
   }

   void UpdateForces(Configuration & conf)
   {
    BasicParticleSet & atoms = conf.Atoms();
    for (long i=0;i<atoms.Size();++i) atoms[i].Acceleration() = Vector(0.0, 0.0, 0.0);
    conf.Virial() = 0.0;
    for (int p=0;p<3;++p)
      for (int q=0;q<3;++q) conf.StressTensor().Set(q, p, 0.0);
    CellManager & cm = conf.GetCellManager();
    cm.UpdateCell(conf);
    for (int p=0;p<Size();++p) (*this)[p].UpdateForces(conf);
    conf.SetTag(conf, Tag("potential-energy"), energy(conf));
   }

   const Potential & PotentialForElements(int s1, int s2) const
   {
    for (int p=0;p<Size();++p)
    {
     if ((*this)[p].AppliesTo(s1, s2)) return (*this)[p];
    }
    throw RuntimeError("No potential assigned to atomic numbers "+ToString<int>(s1)+" and "+ToString<int>(s2));
   }

};

}

#endif

