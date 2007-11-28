//
//
//

#include <lpmd/potential.h>

using namespace lpmd;

Potential::Potential() { }

Potential::~Potential() { }

void Potential::SetValidSpecies(int s1, int s2) { spc_sum = s1+s2; }

bool Potential::AppliesTo(int s1, int s2) const { return ((s1+s2) == spc_sum); }

