//
//
//

#ifndef __LPMD_SIMULATION_H__
#define __LPMD_SIMULATION_H__

#include <lpmd/nonorthogonalcell.h>
#include <lpmd/atom.h>
#include <lpmd/array.h>
#include <lpmd/particleset.h>
#include <lpmd/properties.h>
#include <lpmd/neighbor.h>
#include <lpmd/cellmanager.h>
#include <lpmd/integrator.h>

// FIXME: no corresponde aqui
const double kboltzmann = 8.6173422E-05;

namespace lpmd
{
 class Potential;    // forward

template <typename AtomContainer, typename CellType> class Simulation
{
 public:
  Simulation(): atoms(0), cell(0), velocitiesSet(false), initialized(false)
  {
   // This hints Simulation that the number of atoms is variable
   std::cerr << "DEBUG Calling Simulation constructor, no arguments\n";
   atoms = new AtomContainer();  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
  }

  Simulation(long int natoms): atoms(0), cell(0), velocitiesSet(false), initialized(false)
  {
   // This fixes the number of atoms for Simulation
   std::cerr << "DEBUG Calling Simulation constructor, (long int natoms)\n";
   atoms = new AtomContainer(natoms);  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
  }

  Simulation(long int natoms, const AtomInterface & t): atoms(0), cell(0), velocitiesSet(false), initialized(false)
  {
   // This fixes the number of atoms for Simulation
   std::cerr << "DEBUG Calling Simulation constructor, (long int natoms, const AtomInterface &)\n";
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
  integ->Advance(*atoms, *cell, potarray[0]);
  // Do step
 }

 void SetIntegrator(lpmd::Integrator & itg) { integ = &itg; }

 void SetCellManager(lpmd::CellManager & cm) { cellman = &cm; }

 lpmd::Integrator & Integrator() { return (*integ); }
 
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

  void Initialize() { integ->Initialize(*atoms, *cell, potarray[0]); }

  CellType * cell;
  AtomContainer * atoms;
  Array<Potential &> potarray;
  lpmd::Integrator * integ;
  lpmd::CellManager * cellman;
  bool velocitiesSet;
  bool initialized;
  Array<Neighbor> neighlist;
};

} // lpmd

#endif


