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

void Potential::Initialize(Configuration & conf) { energycache = 0; }

void Potential::SetValidSpecies(int s1, int s2) { spc_sum = s1+s2; }

bool Potential::AppliesTo(int s1, int s2) const { return ((s1+s2) == spc_sum); }

