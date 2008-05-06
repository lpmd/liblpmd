//
//
//

#ifndef __LPMD_PAIRPOTENTIAL_H__
#define __LPMD_PAIRPOTENTIAL_H__

/**
 *  \file pairpotential.h
 *  \brief Declaraci�n de la clase PairPotential.
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
  /// Constructor por omisi�n.
  PairPotential();
  /// Destructor.
  virtual ~PairPotential();

  /// Implementa Potential::energy .
  double energy(SimulationCell & sc) ;
  /// Implementa Potential::UpdateForces .
  void UpdateForces(SimulationCell & sc);

  /** M�todo "callback", el cual debe devolver la energ�a potencial para un par de part�culas.
   * 
   *  \param r La distancia entre dos part�culas.
   *  \return La energ�a potencial del par de part�culas.
   */
  virtual double pairEnergy(const double & r) const = 0;

  /** M�todo "callback", el cual debe devolver el vector fuerza para un par de part�culas.
   *
   *  \param r El vector que separa dos part�culas.
   *  \return El vector fuerza entre el par de part�culas.
   */
  virtual Vector pairForce(const Vector & r) const = 0;
};

} // lpmd

#endif


