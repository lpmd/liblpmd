//
//
//

#include <lpmd/md.h>

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
 output << step << std::endl ;
 output << cell.GetVector(0) << std::endl;
 output << cell.GetVector(1) << std::endl;
 output << cell.GetVector(2) << std::endl;
 cell.WriteAll(output);
 output.close();
}

void MD::LoadDump(std::string filename)
{
 SimulationCell & cell = GetCell();
 std::ifstream input(filename.c_str());
 Vector a, b, c;
 input >> step ;
 input >> a ;
 cell.SetVector(0,a);
 input >> b ;
 cell.SetVector(1,b);
 input >> c ;
 cell.SetVector(2,c);
 int N=0;
 input >> N;
 cell.Initialize(N);
 for(int i=0;i<N;i++) 
 {
  Atom at;
  input >> at;
  cell.AppendAtom(at);
 }
 input.close();
}


