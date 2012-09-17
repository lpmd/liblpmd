//
//
//

#ifndef __LPMD_METALPOTENTIAL_H__
#define __LPMD_METALPOTENTIAL_H__

#include <lpmd/vector.h>
#include <lpmd/potential.h>

namespace lpmd
{

class MetalPotential: public Potential
{
 public:
  MetalPotential();
  MetalPotential(const MetalPotential & mp);

  virtual ~MetalPotential();

  void Initialize(Configuration & conf);

  double energy(Configuration & conf);
  double AtomEnergy(Configuration & conf, long i);
  void UpdateForces(Configuration & conf);

  virtual double pairEnergy(const double & r) const = 0;

  virtual double rhoij(const double & r) const = 0;
  
  virtual double F(const double & rho) const = 0;

  virtual Vector PairForce(const Vector & normr, const double & mod) const = 0;

  virtual Vector ManyBodies(const Vector & normrij, const double & rhoi, const double & rhoj, const double & mod) const = 0;

  virtual Vector UpdateCorrections(const double & rho, const int & N, const double & sinv) const = 0;

 private:
  double *rho,du,drho,dvir;
  bool initial;
};

} // lpmd

#endif


