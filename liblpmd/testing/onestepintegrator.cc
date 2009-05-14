//
//
//

#include <lpmd/onestepintegrator.h>
#include <lpmd/simulation.h>
#include <lpmd/potential.h>
#include <lpmd/atom.h>

using namespace lpmd;

void OneStepIntegrator::Advance(Simulation & sim, Potential & p)
{
 p.UpdateForces(sim);
 // Setea a cero las aceleraciones de los atomos con fixedvel
 const Vector aczero(0.0, 0.0, 0.0);
 //FIXME : AtomType
 /*
 for (long int i=0;i<sc.Size();++i) 
 {
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedvel")) sc.SetAcceleration(i, aczero);
 }
 */
 BasicParticleSet & atoms = sim.Atoms();
 for (long int i=0;i<atoms.Size();++i)
 {
  const Atom & at = atoms[i];
  //FIXME : AtomType
  //if (at.IsTypeSet() && at.Type().GetBool("fixedpos")) continue;
  //else Advance(sc, i);
  Advance(sim, i);
 }
}

