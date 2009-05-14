//
//
//

#include <lpmd/potential.h>
#include <lpmd/simulation.h>
#include <lpmd/util.h>

#include <cmath>

using namespace lpmd;

Potential::Potential() { rcutoff = HUGE_VAL; }

Potential::~Potential() { }

void Potential::Initialize(Simulation & sim) { energycache = 0; innersim = &sim; }

double Potential::energy(Simulation & sim) { return energy(sim.Atoms(), sim.Cell()); }

void Potential::UpdateForces(Simulation & sim) { UpdateForces(sim.Atoms(), sim.Cell()); }

void Potential::SetValidSpecies(int s1, int s2) { spc_sum = s1+s2; }

bool Potential::AppliesTo(int s1, int s2) const { return ((s1+s2) == spc_sum); }

HorrendousForce::HorrendousForce(double mag): Error("Horrendous force! |F| = "+ToString<double>(mag)+" eV/angstrom") { }

