//
//
//

#include <lpmd/integrator.h>
#include <lpmd/util.h>

using namespace lpmd;

Integrator::Integrator() 
{ 
 oldcell = NULL;
}

Integrator::~Integrator() 
{ 
 if (oldcell != NULL) delete oldcell;
}

void Integrator::UseOldCell(SimulationCell & sc) 
{
 if (oldcell == NULL)
 {
  oldcell = new SimulationCell(sc);
  oldcell->UseDistanceCache(false);
  GoBack(*oldcell);
 }
}

SimulationCell & Integrator::OldCell() const { return *(oldcell); }

void Integrator::Initialize(SimulationCell & sc, Potential & p) { }

void Integrator::GoBack(SimulationCell & sc)
{
 Atom now;
 Vector newpos, newvel;
 for (int i=0;i<sc.Size();++i)
 {
  now = sc.GetAtom(i);
  newpos = now.Position() - now.Velocity()*dt;
  newvel = now.Velocity() - now.Acceleration()*dt;
  sc.SetPosition(i, newpos);
  sc.SetVelocity(i, newvel);
 }
}

double Integrator::Timestep() const { return dt; }


