//
//
//

#ifndef __LPMD_INTEGRATOR_H__
#define __LPMD_INTEGRATOR_H__

/**
 *  \file integrator.h
 *  \brief Declaración de la clase Integrator.
 *  \author GNM
 */

#include "simulationcell.h"
#include "potential.h"

namespace lpmd
{

/**
 * Integrator es la clase base de la cual deben derivar todos los métodos de integración para poder ser usados por LPMD.
 */
class Integrator
{
 public:
   /// Constructor por omisión.
   Integrator();
   /// Destructor. 
   virtual ~Integrator();

   /** Mueve la celda de simulación un paso atrás en el tiempo.
    *
    * Internamente realiza el paso hacia atrás usando integración de Euler.
    */
   void GoBack(SimulationCell & sc);

   /// Devuelve el paso de tiempo actual.
   double Timestep() const;

   /// Prepara el integrador para usar una copia de la celda de simulación, por ejemplo para guardar temporalmente las coordenadas antiguas.
   void UseOldCell(SimulationCell & sc);

   /// Devuelve una referencia a la copia auxiliar de la celda de simulación (requerida previamente con Integrator::UseOldCell)
   SimulationCell & OldCell() const;

   /// Método "callback" en el cual la inicialización del integrador debe implementarse.
   /// 
   /// \param sc Referencia a la celda de simulación.
   /// \param p Referencia al potencial que actúa sobre la celda de simulación.
   virtual void Initialize(SimulationCell & sc, Potential & p);

   /// Método "callback" en el cual se implementa la integración misma.
   ///
   /// Su objetivo es mover las partículas de la celda de simulación un paso adelante en el tiempo.
   /// \param sc Referencia a la celda de simulación.
   /// \param p Referencia al potencial que actúa sobre la celda de simulación.
   virtual void Advance(SimulationCell & sc, Potential & p) = 0;

 protected:
   /// Valor interno del paso actual de tiempo para la integración.
   double dt;

 private:
   SimulationCell *oldcell;
};

} // lpmd

#endif


