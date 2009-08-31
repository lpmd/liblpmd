//
//
//

#include <lpmd/systemmodifier.h>

using namespace lpmd;

SystemModifier::SystemModifier() { }

SystemModifier::~SystemModifier() { }

void SystemModifier::Apply(Simulation & sim) { assert(&sim != 0); }//icc 869

