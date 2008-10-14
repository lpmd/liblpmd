//
//
//

#ifndef __LPMD_POTENTIAL_H__
#define __LPMD_POTENTIAL_H__

/**
 *  \file potential.h
 *  \brief Declaración de la clase Potential.
 *  \author GNM
 */

#include <lpmd/error.h>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace lpmd
{

 class SimulationCell;              // forward declaration

/** 
 * Potential es la clase base de la cual deben derivar todos los potenciales para ser usados por LPMD.  
 */
class Potential
{
 public:
   /// Constructor por omisión.
   Potential();
   /// Destructor.
   virtual ~Potential();

   /** Indica las especies atómicas para las cuales el potencial es aplicable. 
    *
    *  Cualquier otro par de especies atómicas debería ser ignorado tanto en el cálculo de fuerzas
    *  como en el cálculo de la energía potencial del sistema. El orden entre s1 y s2 no es importante.
    */
   void SetValidSpecies(int s1, int s2);

   /// Devuelve true si el potencial aplica a las especies s1 y s2.
   bool AppliesTo(int s1, int s2) const;

   void SetCutoff(double rc) { rcutoff = rc; }

   double GetCutoff() const { return rcutoff; }

   /// Método de "callback" opcional para los potenciales que necesiten inicializar algun tipo de cache
   virtual void Initialize(SimulationCell & sc);

   /// Método de "callback" que debe devolver la energía potencial de la celda de simulación.
   virtual double energy(SimulationCell & sc) = 0;

   /** Método de "callback" en el cual el ciclo de fuerzas debe ser implementado.
    *
    * Las aceleraciones para cada átomo en la celda de simulación deben ser actualizadas. 
    */
   virtual void UpdateForces(SimulationCell & sc) = 0;

 private:
   int spc_sum;
   double rcutoff;
};

 class HorrendousForce: public Error
 {
  public:
    HorrendousForce(double mag);
 };

} // lpmd

#endif


