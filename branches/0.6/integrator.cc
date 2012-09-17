//
//
//

#include <lpmd/integrator.h>
#include <lpmd/potential.h>
#include <lpmd/storedconfiguration.h>
#include <lpmd/util.h>

using namespace lpmd;

Integrator::Integrator(): oldconfig(0) { }

Integrator::~Integrator() 
{ 
 delete oldconfig;
}

void Integrator::UseOldConfig(Configuration & conf) 
{
 if (oldconfig == 0)
 {
  oldconfig = new StoredConfiguration(conf);
  oldconfig->SetCellManager(conf.GetCellManager());
  GoBack(*oldconfig);
 }
}

Configuration & Integrator::OldConfig() const { return *(oldconfig); }

void Integrator::Initialize(Simulation & sim, Potential & p) { assert(&sim != 0); assert(&p != 0); }//icc 869

void Integrator::GoBack(Configuration & conf)
{
 BasicParticleSet & atoms = conf.Atoms();
 BasicCell & cell = conf.Cell();
 Vector newpos, newvel;
 for (long int i=0;i<atoms.Size();++i)
 {
  const Atom & now = atoms[i];
  newpos = now.Position() - now.Velocity()*dt;
  newvel = now.Velocity() - now.Acceleration()*dt;
  atoms[i].Position() = cell.FittedInside(newpos);
  atoms[i].Velocity() = newvel;
 }
}

double Integrator::Timestep() const { return dt; }

