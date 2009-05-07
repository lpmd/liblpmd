//
//
//

#ifndef __LPSimulation_SIMULATION_H__
#define __LPSimulation_SIMULATION_H__

#include <string>

namespace lpmd
{

 class SimulationCell;                // forward declaration
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

   long CurrentStep() const; 

   void DoStep();
   void DoSteps(int nsteps);

   void Dump(std::string filename);
   void LoadDump(std::string filename);

 protected:
   /// Valor interno del paso de simulación actual.
   long step;

 private:
   // Private implementation pointer
   class SimulationImpl * md_impl;
};

} // lpmd

#endif


