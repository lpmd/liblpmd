//
//
//

#include <lpmd/onestepintegrator.h>

using namespace lpmd;

void OneStepIntegrator::Advance(SimulationCell & sc, Potential & p)
{
 p.UpdateForces(sc);
 Advance(sc);
}

