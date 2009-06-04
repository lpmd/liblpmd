//
//
//

#include <lpmd/systemfilter.h>
#include <lpmd/particleset.h>

using namespace lpmd;

SystemFilter::SystemFilter() { }

SystemFilter::~SystemFilter() { }

void SystemFilter::Apply(Simulation & sim) 
{ 
 BasicParticleSet & atoms = sim.Atoms();
 Selector<BasicParticleSet> & selector = CreateSelector();
 ParticleSet filtered(selector.SelectFrom(atoms));
 atoms.Clear();
 for (long int i=0;i<filtered.Size();++i) atoms.Append(filtered[i]);
}

