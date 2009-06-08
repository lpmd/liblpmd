//
//
//

#include <lpmd/twostepintegrator.h>
#include <lpmd/simulation.h>
#include <lpmd/potential.h>

using namespace lpmd;

#warning desactivados los AtomType
void TwoStepIntegrator::Advance(Simulation & sim, Potential & p)
{
 p.UpdateForces(sim);
 BasicParticleSet & atoms = sim.Atoms();
 const Vector aczero(0.0, 0.0, 0.0);
 /*
 for (long int i=0;i<sc.Size();++i) 
 {
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedvel")) sc.SetAcceleration(i, aczero);
 }
 */
 for (long int i=0;i<atoms.Size();++i) 
 { 
  //const Atom & at = atoms[i];
  //if (at.IsTypeSet() && at.Type().GetBool("fixedpos")) continue;
  //else AdvancePosition(sc, i);
  AdvancePosition(sim, i);
 }
 p.UpdateForces(sim);
 /*
 for (long int i=0;i<sc.Size();++i) 
 {
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedvel")) sc.SetAcceleration(i, aczero);
 }
 */
 for (long int i=0;i<atoms.Size();++i) 
 { 
  //const Atom & at = atoms[i];
  //if (at.IsTypeSet() && at.Type().GetBool("fixedpos")) continue;
  //else AdvanceVelocity(sc, i);
  AdvanceVelocity(sim, i);
 }
}

