//
//
//

#ifndef __LPMD_PAIRPOTENTIAL_H__
#define __LPMD_PAIRPOTENTIAL_H__

/**
 *  \file pairpotential.h
 *  \brief Declaración de la clase PairPotential.
 *  \author GNM
 */

#include <lpmd/vector.h>
#include <lpmd/potential.h>

namespace lpmd
{

/**
 * PairPotential es la clase base para los potenciales de pares en LPMD.
 */

class PairPotential: public Potential
{
 public:
  /// Constructor por omisión.
  PairPotential();
  /// Destructor.
  virtual ~PairPotential();

  /// Implementa Potential::energy .
  double energy(SimulationCell & sc) ;
  /// Implementa Potential::UpdateForces .
  void UpdateForces(SimulationCell & sc);

  /** Método "callback", el cual debe devolver la energía potencial para un par de partículas.
   * 
   *  \param r La distancia entre dos partículas.
   *  \return La energía potencial del par de partículas.
   */
  virtual double pairEnergy(const double & r) const = 0;

  /** Método "callback", el cual debe devolver el vector fuerza para un par de partículas.
   *
   *  \param r El vector que separa dos partículas.
   *  \return El vector fuerza entre el par de partículas.
   */
  virtual Vector pairForce(const Vector & r) const = 0;
};

} // lpmd

#endif


