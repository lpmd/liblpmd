//
//
//

#include <lpmd/twostepintegrator.h>
#include <lpmd/potential.h>
#include <lpmd/simulationcell.h>

using namespace lpmd;

void TwoStepIntegrator::Advance(SimulationCell & sc, Potential & p)
{
 p.UpdateForces(sc);
 const Vector aczero(0.0, 0.0, 0.0);
 for (long i=0;i<sc.Size();++i) 
 {
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedvel")) sc.SetAcceleration(i, aczero);
 }
 for (long i=0;i<sc.Size();++i) 
 { 
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedpos")) continue;
  else AdvancePosition(sc, i);
 }
 p.UpdateForces(sc);
 for (long i=0;i<sc.Size();++i) 
 {
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedvel")) sc.SetAcceleration(i, aczero);
 }
 for (long i=0;i<sc.Size();++i) 
 { 
  const Atom & at = sc[i];
  if (at.IsTypeSet() && at.Type().GetBool("fixedpos")) continue;
  else AdvanceVelocity(sc, i);
 }
}

