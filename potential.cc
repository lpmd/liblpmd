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

void Potential::Initialize(Configuration & conf) { assert(&conf != 0); energycache = 0; }//icc 869

// s1 == s2 == -1 is used as a wildcard
void Potential::SetValidSpecies(int s1, int s2) { spc_sum = s1+s2; }

bool Potential::AppliesTo(int s1, int s2) const { return ((spc_sum < 0) || ((s1+s2) == spc_sum)); }

