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
 BasicParticleSet & orig_atoms = sim.OriginalAtoms();
 if (except != "")
 {
  for (long int i=0;i<orig_atoms.Size();++i) 
  {
   if (orig_atoms.Have(orig_atoms[i], Tag(except)) && (bool(Parameter(orig_atoms.GetTag(orig_atoms[i], Tag(except))))))
      filtered->Append(orig_atoms[i]);
  }
 }
 atoms.Clear();
 for (long int i=0;i<filtered->Size();++i) atoms.Append((*filtered)[i]);
 delete filtered;
}

