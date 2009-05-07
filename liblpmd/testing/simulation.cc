//
//
//

#include <lpmd/simulation.h>
#include <lpmd/simulationcell.h>
#include <lpmd/integrator.h>
#include <lpmd/potentialarray.h>

#include <fstream>
#include <cassert>

using namespace lpmd;

// Implementation class for Simulation
class lpmd::SimulationImpl
{
 public:
   SimulationImpl(): itg(0) { }

   SimulationCell *sc, *oldsc;
   Integrator *itg;
   PotentialArray p_array;
};

Simulation::Simulation() 
{ 
 md_impl = new SimulationImpl();
 assert(md_impl->itg == 0);
 step = 0; 
}

Simulation::Simulation(SimulationCell & simcell) 
{ 
 md_impl = new SimulationImpl();
 assert(md_impl->itg == 0);
 SetCell(simcell); 
 step = 0;
}

Simulation::~Simulation() { delete md_impl; }

SimulationCell & Simulation::GetCell() const { return *(md_impl->sc); }

void Simulation::SetCell(SimulationCell & simcell) { md_impl->sc = &simcell; }

void Simulation::Initialize() { }

Integrator & Simulation::GetIntegrator() const 
{
 if (md_impl->itg == 0) throw NoIntegrator(); 
 return *(md_impl->itg); 
}

void Simulation::SetIntegrator(Integrator & integ) 
{ 
 md_impl->itg = &integ; 
 SimulationCell & cell = GetCell();
 PotentialArray & p = GetPotentialArray();
 integ.Initialize(cell, p);
}

PotentialArray & Simulation::GetPotentialArray() { return md_impl->p_array; }

long Simulation::CurrentStep() const { return step; }

void Simulation::DoStep()
{
 SimulationCell & cell = GetCell();
 PotentialArray & pot = GetPotentialArray();
 Integrator & integ = GetIntegrator();
 integ.Advance(cell, pot);
}

void Simulation::DoSteps(int nsteps)
{
 for (step=0;step<nsteps;++step) DoStep();
}

