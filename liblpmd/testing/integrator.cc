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
 // std::cerr << "DEBUG Before UseOldCell\n";
 //std::cerr << " -> " << sc[0].Position() << '\n';
 //std::cerr << " -> " << sc[1].Position() << '\n';
 if (oldcell == NULL)
 {
  //std::cerr << "DEBUG Calling copy constructor on SC... in Integrator::UseOldCell\n";
  oldcell = new SimulationCell(sc);
  //std::cerr << "DEBUG Copy constructor called\n";
  oldcell->SetCellManager(sc.GetCellManager()); // No deberia ser problema que ambas sc compartan el mismo CellManager
  GoBack(*oldcell);
 }
 //std::cerr << "DEBUG After UseOldCell\n";
 //std::cerr << " -> " << sc[0].Position() << '\n';
 //std::cerr << " -> " << sc[1].Position() << '\n';
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


