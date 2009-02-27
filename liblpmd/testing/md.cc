//
//
//

#include <lpmd/md.h>
#include <lpmd/simulationcell.h>
#include <lpmd/integrator.h>
#include <lpmd/potentialarray.h>

#include <fstream>

using namespace lpmd;

// Implementation class for MD
class lpmd::MDImpl
{
 public:
   SimulationCell *sc, *oldsc;
   Integrator *itg;
   PotentialArray p_array;
};

MD::MD() 
{ 
 md_impl = new MDImpl();
 step = 0; 
}

MD::MD(SimulationCell & simcell) 
{ 
 md_impl = new MDImpl();
 SetCell(simcell); 
 step = 0;
}

MD::~MD() { delete md_impl; }

SimulationCell & MD::GetCell() const { return *(md_impl->sc); }

void MD::SetCell(SimulationCell & simcell) { md_impl->sc = &simcell; }

void MD::Initialize() { }

Integrator & MD::GetIntegrator() const { return *(md_impl->itg); }

void MD::SetIntegrator(Integrator & integ) 
{ 
 md_impl->itg = &integ; 
 SimulationCell & cell = GetCell();
 PotentialArray & p = GetPotentialArray();
 integ.Initialize(cell, p);
}

PotentialArray & MD::GetPotentialArray() { return md_impl->p_array; }

long MD::CurrentStep() const { return step; }

void MD::DoStep()
{
 SimulationCell & cell = GetCell();
 PotentialArray & pot = GetPotentialArray();
 Integrator & integ = GetIntegrator();
 integ.Advance(cell, pot);
}

void MD::DoSteps(int nsteps)
{
 for (step=0;step<nsteps;++step) DoStep();
}

void MD::Dump(std::string filename)
{
 SimulationCell & cell = GetCell();
 std::ofstream output(filename.c_str());
 output << step << std::endl;
 for (int q=0;q<3;++q) output << cell.GetVector(q) << std::endl;
 cell.WriteAll(output);
 output.close();
}

void MD::LoadDump(std::string filename)
{
 SimulationCell & cell = GetCell();
 std::ifstream input(filename.c_str());
 input >> step;
 for (int q=0;q<3;++q)
 {
  Vector tmp;
  input >> tmp;
  cell.SetVector(q, tmp);
 }
 int N=0;
 input >> N;
 for(int i=0;i<N;i++) 
 {
  Atom * at = new Atom();
  input >> (*at);
  cell.Create(at);
 }
 input.close();
}


