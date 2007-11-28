//
//
//

#ifndef __LPMD_BASICPROPERTIES_H__
#define __LPMD_BASICPROPERTIES_H__

#include "scalarvalue.h"
#include "instantproperty.h"

namespace lpmd
{

//
// Potential energy of the system
//
class PropPotEnergy: public ScalarValue, public InstantProperty
{
 public:
   PropPotEnergy(): ScalarValue() { }
   ~PropPotEnergy() { }
   //
   void Evaluate(SimulationCell & simcell, Potential & pot) { potenerg = pot.energy(simcell); }

   const double & Value() const { return potenerg; }

 private:
   double potenerg;
};

//
// Kinetic energy of the system
//
class PropKinEnergy: public ScalarValue, public InstantProperty
{
 public:
   PropKinEnergy(): ScalarValue() { }
   ~PropKinEnergy() { }
   // 
   void Evaluate(SimulationCell & simcell, Potential & pot) { kinenerg = simcell.KineticEnergy(); }

   const double & Value() const { return kinenerg; }

 private:
   double kinenerg;  
};

//
// Temperature of the system
// 
class PropTemperature: public ScalarValue, public InstantProperty
{
 public:
   PropTemperature(): ScalarValue() { }
   ~PropTemperature() { }
   //
   void Evaluate(SimulationCell & simcell, Potential & pot) { temp = simcell.Temperature(); }

   const double & Value() const { return temp; }

 private:
   double temp;
};

//
// Magnitude of total momentum of the system
//
class PropMomentum: public ScalarValue, public InstantProperty
{
 public:
   PropMomentum(): ScalarValue() { }
   ~PropMomentum() { }
   // 
   void Evaluate(SimulationCell & simcell, Potential & pot) { mom = simcell.Momentum(); }

   const double & Value() const { return mom; }
 
 private:
   double mom;
};

} // lpmd

#endif


