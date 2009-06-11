/*
 *
 *
 *
 */

#ifndef __LPMD_PROPERTIES_H__
#define __LPMD_PROPERTIES_H__

#define KIN2EV (103.64269)
#define KBOLTZMANN (8.6173422E-05)

#include <lpmd/util.h>
#include <lpmd/configuration.h>
#include <lpmd/potential.h>
#include <lpmd/matrix.h>
#include <lpmd/simulationhistory.h>
#include <lpmd/storedconfiguration.h>

namespace lpmd
{

 template <typename T> double KineticEnergy(const T & atomcont)
 {
  double K = 0.0;
  for (long int i=0;i<atomcont.Size();++i)
      K += 0.5*atomcont[i].Mass()*atomcont[i].Velocity().SquareModule();
  return K*KIN2EV;
 }

 template <typename T> double Temperature(const T & atomcont)
 {
  if (atomcont.Size() == 0) EndWithError("Cannot compute temperature without atoms!");
  return (2.0/3.0)*KineticEnergy(atomcont)/(KBOLTZMANN*double(atomcont.Size()));
 }

 template <typename T, typename V> double Density(const T & atomcont, const V & cell)
 {
  if (atomcont.Size() == 0) return 0.0;
  double total_mass = 0.0;
  for (long int i=0;i<atomcont.Size();++i) total_mass += atomcont[i].Mass();
  return (total_mass/cell.Volume());
 }

 template <typename T> Vector Momentum(const T & atomcont)
 {
  if (atomcont.Size() == 0) return Vector(0.0, 0.0, 0.0);
  Vector p(0.0, 0.0, 0.0);
  for (long int i=0;i<atomcont.Size();++i) p += (atomcont[i].Mass()*atomcont[i].Velocity());
  return p;
 }

 void gdr(lpmd::Configuration & con, lpmd::Potential & pot, long int nb, double rcut, lpmd::Matrix & m);

 void vacf(lpmd::ConfigurationSet & sim, lpmd::Potential & pot, double dt, lpmd::Matrix & m);

}  // lpmd

#endif
