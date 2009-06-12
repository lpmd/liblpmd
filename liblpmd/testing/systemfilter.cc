//
//
//

#include <lpmd/systemfilter.h>
#include <lpmd/particleset.h>

using namespace lpmd;

SystemFilter::SystemFilter(): inverted(false) { }

SystemFilter::~SystemFilter() { }

void SystemFilter::Apply(Simulation & sim) 
{ 
 BasicParticleSet & atoms = sim.Atoms();
 Selector<BasicParticleSet> & selector = CreateSelector();
 ParticleSet * filtered = 0;
 if (!inverted) filtered = new ParticleSet(selector.SelectFrom(atoms));
 else filtered = new ParticleSet(selector.InverseSelectFrom(atoms));
 atoms.Clear();
 for (long int i=0;i<filtered->Size();++i) atoms.Append((*filtered)[i]);
 delete filtered;
}

