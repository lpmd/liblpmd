//
//
//

#include <lpmd/simulation.h>
#include <lpmd/simulationcell.h>
#include <lpmd/integrator.h>
#include <lpmd/potentialarray.h>

#include <fstream>

using namespace lpmd;

// Implementation class for Simulation
class lpmd::SimulationImpl
{
 public:
   SimulationCell *sc, *oldsc;
   Integrator *itg;
   PotentialArray p_array;
};

Simulation::Simulation() 
{ 
 md_impl = new SimulationImpl();
 step = 0; 
}

Simulation::Simulation(SimulationCell & simcell) 
{ 
 md_impl = new SimulationImpl();
 SetCell(simcell); 
 step = 0;
}

Simulation::~Simulation() { delete md_impl; }

SimulationCell & Simulation::GetCell() const { return *(md_impl->sc); }

void Simulation::SetCell(SimulationCell & simcell) { md_impl->sc = &simcell; }

void Simulation::Initialize() { }

Integrator & Simulation::GetIntegrator() const { return *(md_impl->itg); }

void Simulation::SetIntegrator(Integrator & integ) 
{ 
 md_impl->itg = &integ; 
 SimulationCell & cell = GetCell();
 PotentialArray & p = GetPotentialArray();
// std::cerr << "DEBUG ---- Before Integrator::Initialize: atom 0: " << cell[0].Position() << '\n';
// std::cerr << "DEBUG ---- Before Integrator::Initialize: atom 1: " << cell[1].Position() << '\n';
 integ.Initialize(cell, p);
// std::cerr << "DEBUG ++++ After Integrator::Initialize: atom 0: " << cell[0].Position() << '\n';
// std::cerr << "DEBUG ++++ After Integrator::Initialize: atom 1: " << cell[1].Position() << '\n';
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

//FIXME : Comentado por nuevo vector.h para 0.6
void Simulation::Dump(std::string filename)
{
 EndWithError("Simulation::Dump : Metodo vacio");
// SimulationCell & cell = GetCell();
// std::ofstream output(filename.c_str());
// output << step << std::endl;
// for (int q=0;q<3;++q) output << cell.GetVector(q) << std::endl;
// cell.WriteAll(output);
// output.close();
}

//FIXME : Comentado por nuevo vector.h para 0.6
void Simulation::LoadDump(std::string filename)
{
 EndWithError("Simulation::LoadDump : Metodo vacio");
// SimulationCell & cell = GetCell();
// std::ifstream input(filename.c_str());
// input >> step;
// for (int q=0;q<3;++q)
// {
//  Vector tmp;
//  input >> tmp;
//  cell.SetVector(q, tmp);
// }
// int N=0;
// input >> N;
// for(int i=0;i<N;i++) 
// {
//  Atom * at = new Atom();
//  input >> (*at);
//  cell.Create(at);
// }
// input.close();
}


