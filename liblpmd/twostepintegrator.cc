//
//
//

#include <lpmd/twostepintegrator.h>

using namespace lpmd;

void TwoStepIntegrator::Advance(SimulationCell & sc, Potential & p)
{
 p.UpdateForces(sc);
 AdvancePositions(sc);
 p.UpdateForces(sc);
 AdvanceVelocities(sc);
}


