//
//
//

#ifndef __LPMD_PAIRPOTENTIAL_H__
#define __LPMD_PAIRPOTENTIAL_H__

#include <lpmd/vector.h>
#include <lpmd/potential.h>
#include <lpmd/simulation.h>

namespace lpmd
{

class PairPotential: public Potential
{
 public:
  PairPotential();
  virtual ~PairPotential();

  double energy(Configuration & conf);
  double AtomEnergy(Configuration & conf, long i);
  void UpdateForces(Configuration & conf);

  virtual double pairEnergy(const double & r) const = 0;

  virtual Vector pairForce(const Vector & r) const = 0;
};

} // lpmd

#endif


