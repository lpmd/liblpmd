//
//
//

#ifndef __LPMD_INTEGRATOR_H__
#define __LPMD_INTEGRATOR_H__

/**
 *  \file integrator.h
 *  \brief Declaraci�n de la clase Integrator.
 *  \author GNM
 */

#include "simulationcell.h"
#include "potential.h"

namespace lpmd
{

/**
 * Integrator es la clase base de la cual deben derivar todos los m�todos de integraci�n para poder ser usados por LPMD.
 */
class Integrator
{
 public:
   /// Constructor por omisi�n.
   Integrator();
   /// Destructor. 
   virtual ~Integrator();

   /** Mueve la celda de simulaci�n un paso atr�s en el tiempo.
    *
    * Internamente realiza el paso hacia atr�s usando integraci�n de Euler.
    */
   void GoBack(SimulationCell & sc);

   /// Devuelve el paso de tiempo actual.
   double Timestep() const;

   /// Prepara el integrador para usar una copia de la celda de simulaci�n, por ejemplo para guardar temporalmente las coordenadas antiguas.
   void UseOldCell(SimulationCell & sc);

   /// Devuelve una referencia a la copia auxiliar de la celda de simulaci�n (requerida previamente con Integrator::UseOldCell)
   SimulationCell & OldCell() const;

   /// M�todo "callback" en el cual la inicializaci�n del integrador debe implementarse.
   /// 
   /// \param sc Referencia a la celda de simulaci�n.
   /// \param p Referencia al potencial que act�a sobre la celda de simulaci�n.
   virtual void Initialize(SimulationCell & sc, Potential & p);

   /// M�todo "callback" en el cual se implementa la integraci�n misma.
   ///
   /// Su objetivo es mover las part�culas de la celda de simulaci�n un paso adelante en el tiempo.
   /// \param sc Referencia a la celda de simulaci�n.
   /// \param p Referencia al potencial que act�a sobre la celda de simulaci�n.
   virtual void Advance(SimulationCell & sc, Potential & p) = 0;

 protected:
   /// Valor interno del paso actual de tiempo para la integraci�n.
   double dt;

 private:
   SimulationCell *oldcell;
};

} // lpmd

#endif


