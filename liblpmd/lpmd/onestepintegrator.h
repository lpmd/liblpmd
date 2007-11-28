//
//
//

#ifndef __LPMD_ONESTEP_INTEGRATOR_H__
#define __LPMD_ONESTEP_INTEGRATOR_H__

/**
 *  \file onestepintegrator.h
 *  \brief Declaración de la clase OneStepIntegrator.
 *  \author GNM
 */

#include "integrator.h"

namespace lpmd
{

/**
 * OneStepIntegrator es la clase base para todos los métodos de integración de un sólo paso usados por LPMD.
 */
class OneStepIntegrator: public Integrator
{
 public:
   /// Implementa Integrator::Advance
   void Advance(SimulationCell & sc, Potential & p);

   /// Método "callback", el cual debe avanzar las posiciones y velocidades de las partículas en la celda de simulación. 
   virtual void Advance(SimulationCell & sc) = 0;
};

} // lpmd

#endif


