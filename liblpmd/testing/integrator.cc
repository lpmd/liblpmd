//
//
//

#include <lpmd/integrator.h>
#include <lpmd/potential.h>
#include <lpmd/particleset.h>
#include <lpmd/nonorthogonalcell.h>
#include <lpmd/util.h>

using namespace lpmd;

Integrator::Integrator(): oldatoms(0), oldcell(0) { }

Integrator::~Integrator() 
{ 
 delete oldatoms;
 delete oldcell;
}

void Integrator::UseOldCell(BasicParticleSet & atoms, BasicCell & cell) 
{
 if (oldcell == NULL)
 {
  oldatoms = new ParticleSet(atoms.Size());
  oldcell = new NonOrthogonalCell(cell);
  GoBack(*oldatoms, *oldcell);
 }
}

BasicParticleSet & Integrator::OldAtoms() const { return *(oldatoms); }

BasicCell & Integrator::OldCell() const { return *(oldcell); }

void Integrator::Initialize(Simulation & sim, Potential & p) { }

void Integrator::GoBack(BasicParticleSet & atoms, BasicCell & cell)
{
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

