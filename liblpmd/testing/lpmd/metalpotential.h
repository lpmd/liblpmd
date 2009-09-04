//
//
//

#ifndef __LPMD_METALPOTENTIAL_H__
#define __LPMD_METALPOTENTIAL_H__

#include <lpmd/vector.h>
#include <lpmd/potential.h>
#include <lpmd/atompair.h>

namespace lpmd
{
class MetalPotential: public Potential
{
 public:
  MetalPotential();
  MetalPotential(const MetalPotential & mp);

  virtual ~MetalPotential();

  MetalPotential & operator=(const MetalPotential & m);

  MetalPotential & operator=(const Potential & m);

  void Initialize(Configuration & conf);

  double energy(Configuration & conf);
  void UpdateForces(Configuration & conf);

  void VirialEvaluate(Configuration & conf);

  virtual double pairEnergy(const double & r) const = 0;

  virtual double rhoij(const double & r) const = 0;
  
  virtual double F(const double & rho) const = 0;

  virtual double deltarhoi(const double & rhobar) const { return 0.0; }

  virtual double deltaU1(const double & rhobar, const int & N) const { return 0.0; }

  virtual double deltaU2(const double & rhobar, const int & N, const double & rhoi) const { return 0.0; }

  virtual Vector PairForce(const Vector & normr, const double & mod) const = 0;

  virtual Vector ManyBodies(const Vector & normrij, const double & rhoi, const double & rhoj, const double & mod) const = 0;
  
  virtual double VirialContribution(const double &r, const double & rhoi, const double & rhoj) const;
  virtual double VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const;

 private:
  double *rho,*invrho;
  bool initial;
  NeighborList * list;
};

} // lpmd

#endif


