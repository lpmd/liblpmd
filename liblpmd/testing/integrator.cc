//
//
//

#include <lpmd/integrator.h>
#include <lpmd/simulationcell.h>
#include <lpmd/potential.h>
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
  oldcell->SetCellManager(sc.GetCellManager()); // No deberia ser problema que ambas sc compartan el mismo CellManager
  GoBack(*oldcell);
 }
}

SimulationCell & Integrator::OldCell() const { return *(oldcell); }

void Integrator::Initialize(SimulationCell & sc, Potential & p) { }

void Integrator::GoBack(SimulationCell & sc)
{
 Vector newpos, newvel;
 for (unsigned long int i=0;i<sc.size();++i)
 {
  const Atom & now = sc[i];
  newpos = now.Position() - now.Velocity()*dt;
  newvel = now.Velocity() - now.Acceleration()*dt;
  sc.SetPosition(i, newpos);
  sc.SetVelocity(i, newvel);
 }
}

double Integrator::Timestep() const { return dt; }


