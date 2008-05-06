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

#include <lpmd/integrator.h>

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

   /// Metodo "callback", el cual debe avanzar la posicion y velocidad de una particula en la celda de simulacion.
   virtual void Advance(SimulationCell & sc, long i) = 0;
};

} // lpmd

#endif


