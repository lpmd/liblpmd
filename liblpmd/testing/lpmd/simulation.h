//
//
//

#ifndef __LPSimulation_SIMULATION_H__
#define __LPSimulation_SIMULATION_H__

#include <lpmd/error.h>
#include <lpmd/vector.h>
#include <lpmd/simulationcell.h>
#include <string>

namespace lpmd
{

 class PotentialArray;                // forward declaration
 class Integrator;                    // forward declaration

class Simulation
{
 public:
   Simulation();
   Simulation(SimulationCell & simcell);
   virtual ~Simulation();

   virtual void Initialize();
  
   void SetCell(SimulationCell & simcell);
   SimulationCell & GetCell() const;

   PotentialArray & GetPotentialArray();

   void SetIntegrator(Integrator & integ);
   Integrator & GetIntegrator() const;

   void InitVelocities()
   {
    SimulationCell & cell = GetCell();
    Vector vel, totalp;
    long nparts = cell.size();
    double targettemp = 1.0;
    double Kpart = (3.0/2.0)*kboltzmann*targettemp;
    for (int i=0;i<nparts;++i)
    {
     double v2 = 2.0*Kpart/cell[i].Mass();
     vel = RandomVector(sqrt(v2));
     totalp += vel*cell[i].Mass();
     cell[i].Velocity() = vel;
    }
    totalp = totalp/nparts;
    for (int i=0;i<nparts;++i)
    {
     double mass = cell[i].Mass();
     cell[i].Velocity() -= (totalp/mass);
    }
   }

   void SetTemperature(double temp, double dt=1.0, double tau=1.0)
   {
    Vector vel;
    SimulationCell & cell = GetCell();
    double xi, ti = Temperature(cell);
    for (unsigned long int i=0;i<cell.size();++i)
    {
     xi = sqrt(1.0 + (double(dt)/tau)*(temp/ti - 1.0));
     cell[i].Velocity() *= xi;
    }
   }

   long CurrentStep() const; 

   void DoStep();
   void DoSteps(int nsteps);

 private:
   // Private implementation pointer
   class SimulationImpl * md_impl;
   long step;
};

class NoIntegrator: public Error
{
 public:
   NoIntegrator(): Error("No integrator defined") { }

};

class NoSimulationCell: public Error
{
 public:
   NoSimulationCell(): Error("No simulationcell defined") { }

};

} // lpmd

#endif


