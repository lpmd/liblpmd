//
//
//

#ifndef __LPMD_ONESTEP_INTEGRATOR_H__
#define __LPMD_ONESTEP_INTEGRATOR_H__

/**
 *  \file onestepintegrator.h
 *  \brief Declaraci�n de la clase OneStepIntegrator.
 *  \author GNM
 */

#include "integrator.h"

namespace lpmd
{

/**
 * OneStepIntegrator es la clase base para todos los m�todos de integraci�n de un s�lo paso usados por LPMD.
 */
class OneStepIntegrator: public Integrator
{
 public:
   /// Implementa Integrator::Advance
   void Advance(SimulationCell & sc, Potential & p);

   /// M�todo "callback", el cual debe avanzar las posiciones y velocidades de las part�culas en la celda de simulaci�n. 
   virtual void Advance(SimulationCell & sc) = 0;
};

} // lpmd

#endif


