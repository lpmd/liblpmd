//
//
//

#ifndef __LPMD_SIMULATION_H__
#define __LPMD_SIMULATION_H__

#include <lpmd/cell.h>
#include <lpmd/atom.h>
#include <lpmd/array.h>
#include <lpmd/particleset.h>
#include <lpmd/potential.h>
#include <lpmd/properties.h>
#include <lpmd/neighbor.h>
#include <lpmd/cellmanager.h>
#include <lpmd/integrator.h>
#include <lpmd/error.h>

// FIXME: no corresponde aqui
const double kboltzmann = 8.6173422E-05;

namespace lpmd
{
 class NoIntegrator: public Error
 {
  public:
    NoIntegrator(): Error("No integrator defined") { }
 };


class PotentialArray: public Array<Potential &>, public Potential
{
 public:
   double energy(BasicParticleSet & atoms, BasicCell & cell)
   {
    double en = 0.0;
    for (int p=0;p<Size();++p) en += (*this)[p].energy(atoms, cell);
    return en;
   }

   void UpdateForces(BasicParticleSet & atoms, BasicCell & cell)
   {
    for (int p=0;p<Size();++p) (*this)[p].UpdateForces(atoms, cell);
   }
};

template <typename AtomContainer=lpmd::ParticleSet, typename CellType=lpmd::Cell> class Simulation
{
 public:
  Simulation(): atoms(0), cell(0), integ(0), velocitiesSet(false), initialized(false), step(0)
  {
   // This hints Simulation that the number of atoms is variable
   atoms = new AtomContainer();  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
  }

  Simulation(long int natoms): atoms(0), cell(0), integ(0), velocitiesSet(false), initialized(false), step(0)
  {
   // This fixes the number of atoms for Simulation
   atoms = new AtomContainer(natoms);  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
  }

  Simulation(long int natoms, const AtomInterface & t): atoms(0), cell(0), integ(0), velocitiesSet(false), initialized(false), step(0)
  {
   // This fixes the number of atoms for Simulation
   atoms = new AtomContainer(natoms, t);  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
  }

  ~Simulation()
  {
   delete atoms;
   delete cell;
  }

 void SetTemperature(double temp) 
 {  
  if (! velocitiesSet) InitVelocities();
  double ti = Temperature(*atoms);
  for (unsigned long int i=0;i<atoms->Size();++i) (*atoms)[i].Velocity() *= sqrt(temp/ti);
 }

 CellType & Cell() { return (*cell); }

 AtomContainer & Atoms() { return (*atoms); }

 Array<Neighbor> & NeighborList(long int i, bool full, double rcut)
 {
  cellman->BuildNeighborList(*atoms, *cell, i, neighlist, full, rcut);
  return neighlist;
 }

 Array<Potential &> & Potentials() { return potarray; }

 void DoStep() 
 {  
  if (! initialized) Initialize();
  integ->Advance(*atoms, *cell, potarray);
  step++;
 }
 
 inline void DoSteps(long int n) 
 { 
  for (int q=0;q<n;++q) DoStep();
 }

 long int CurrentStep() const { return step; }

 void SetIntegrator(lpmd::Integrator & itg) { integ = &itg; }

 void SetCellManager(lpmd::CellManager & cm) { cellman = &cm; }

 lpmd::Integrator & Integrator() 
 { 
  if (integ == 0) throw NoIntegrator();
  return (*integ); 
 }
 
 lpmd::CellManager & CellManager() { return (*cellman); }

 private:
  void InitVelocities() 
  { 
   Vector vel, totalp;
   long nparts = atoms->Size();
   double targettemp = 1.0;
   double Kpart = (3.0/2.0)*kboltzmann*targettemp;
   for (int i=0;i<nparts;++i)
   {
    double v2 = 2.0*Kpart/(*atoms)[i].Mass();
    vel = RandomVector(sqrt(v2));
    totalp += vel*(*atoms)[i].Mass();
    (*atoms)[i].Velocity() = vel;
   }
   totalp = totalp/nparts;
   for (int i=0;i<nparts;++i)
   {
    double mass = (*atoms)[i].Mass();
    (*atoms)[i].Velocity() -= (totalp/mass);
   }
  }

  void Initialize() 
  { 
   integ->Initialize(*atoms, *cell, potarray); 
   step = 0;
   initialized = true;
  }

  CellType * cell;
  AtomContainer * atoms;
  PotentialArray potarray;
  lpmd::Integrator * integ;
  lpmd::CellManager * cellman;
  bool velocitiesSet;
  bool initialized;
  Array<Neighbor> neighlist;
  long int step;
};

} // lpmd

#endif


