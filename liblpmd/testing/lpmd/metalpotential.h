//
//
//

#ifndef __LPMD_METALPOTENTIAL_H__
#define __LPMD_METALPOTENTIAL_H__

/**
 *  \file metalpotential.h
 *  \brief Declaración de la clase MetalPotential.
 *  \author GNM
 */

#include <lpmd/vector.h>
#include <lpmd/potential.h>

namespace lpmd
{

/**
 * MetalPotential es la clase base para los potenciales de tipo Metalico en LPMD
 */
class MetalPotential: public Potential
{
 public:
  /// Constructor por omisión.
  MetalPotential();
  MetalPotential(const MetalPotential & mp);

  /// Destructor.
  virtual ~MetalPotential();

  /// Implementa Potential::Initialize
  void Initialize(SimulationCell & sc);

  /// Implementa Potential::energy .
  double energy(SimulationCell & sc) ;
  /// Implementa Potential::UpdateForces .
  void UpdateForces(SimulationCell & sc);
  /// Declara contribucion al Virial, MetalPotential::VirialEvaluate()
  void VirialEvaluate(SimulationCell & sc);

  /** Método "callback", el cual debe devolver la energía potencial para un par de partículas.
   * 
   *  \param r La distancia entre dos partículas.
   *  \return La energía potencial del par de partículas.
   */
  virtual double pairEnergy(const double & r) const = 0;

  /** Metodo "callback", devuelve la densidad de pares.
   * \param r es la disntaica entre un par de particulas.
   * \return la densidad.
   */
  virtual double rhoij(const double & r) const = 0;
  
  /** Metodo "callback, devuelve la energia F, relacionada con la densidad.
   * \param rho es la densidad de la particula.
   * \return la energia.
   */
  virtual double F(const double & rho) const = 0;

  /* Metodo callback, para correccion de la densidad.
   * \param rhobar es la densidad del sistema.
   * \param N es el numero total de atomos de la celda.
   * \return correccion a la densidad.
   */
  virtual double deltarhoi(const double & rhobar, const int & N) const = 0;

  /* Metodo callback, para correccion de la energia.
   * \param rhobar es la densidad del sistema.
   * \param es el numero total de atomos de la celda
   * \return correccion ala energia de U1.
   */
  virtual double deltaU1(const double & rhobar, const int & N) const = 0;

  /* Metodo callback, para correccion a la energia.
   * \param rhobar, rhoi son las densidades del sistema y la sin 
   * corregir.
   * \param N es el numero total de atomos de la celda.
   * \return correccion a la energia U2. No se necesita si se corrige la densidad!!!.
   */
  virtual double deltaU2(const double & rhobar, const int & N, const double & rhoi) const = 0;

  /** Metodo callback, metodo para la fuerza de pares de particulas.
   * \param r El vector qu separa dos particulas.
   * \param N Numero total de atomos de la celda.
   * \param rhoi la densidad local del atomo i.
   * \return El vector fuerza entre ambas particulas.
   */
  virtual Vector PairForce(const Vector & r) const = 0;

  /** Método "callback", el cual debe devolver el vector fuerza para un par de partículas.
   *
   *  \param r El vector que separa dos partículas.
   *  \return El vector fuerza entre el par de partículas.
   */
  virtual Vector ManyBodies(const Vector & rij, const double & rhoi, const double & rhoj) const = 0;
  
  /* Metodo callback, para Contribucion al Virial incluye ademas la correccion al virial
   * \param r El vector que separa dos particulas
   * \param rhoi Densidad local de particula i
   * \param rhoj Densidad local de particula j
   * \return El vector fuerza entre ambas.
   */
  virtual double VirialContribution(const double &r, const double & rhoi, const double & rhoj) const;
  virtual double VirialCorrection(const double & rhobar, const int & N, const double & rhoi) const;

 private:
  double *rho;
  bool initial;
};

} // lpmd

#endif


