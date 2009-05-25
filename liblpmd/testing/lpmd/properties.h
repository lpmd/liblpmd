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

 lpmd::Matrix* gdr(lpmd::Configuration & con, lpmd::Potential & pot, long int nb, double rcut);

}  // lpmd

#endif
