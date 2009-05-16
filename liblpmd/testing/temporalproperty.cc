//
//
//

#include <lpmd/temporalproperty.h>
#include <lpmd/simulationhistory.h>

using namespace lpmd;

TemporalProperty::TemporalProperty() { }

TemporalProperty::TemporalProperty(const SimulationHistory & hist, Potential & pot) { }

TemporalProperty::~TemporalProperty() { }

void TemporalProperty::Evaluate(const SimulationHistory & hist, Potential & pot) { }

