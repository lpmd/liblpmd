//
//
//

/**
 *  \file md.h
 *  \brief Declaración de la clase MD.
 *  \author GNM
 */

#ifndef __LPMD_MD_H__
#define __LPMD_MD_H__

#include "simulationcell.h"
#include "potentialarray.h"
#include "integrator.h"

namespace lpmd
{

/**
 *  MD representa un objeto que implementa de forma básica los ciclos de dinámica molecular.
 *
 *  MD es capaz de conectar un potencial y un integrador y hacerlos actuar sobre una celda de simulación. 
 */
class MD
{
 public:
   MD();
   MD(SimulationCell & simcell);
   virtual ~MD();

   virtual void Initialize();
  
   void SetCell(SimulationCell & simcell);
   SimulationCell & GetCell() const;

   PotentialArray & GetPotentialArray();

   void SetIntegrator(Integrator & integ);
   Integrator & GetIntegrator() const;

   long CurrentStep() const; 

   void DoStep();
   void DoSteps(int nsteps);

   void Dump(std::string filename);
   void LoadDump(std::string filename);

 protected:
   /// Valor interno del paso de simulación actual.
   long step;

 private:
   // Private implementation pointer
   class MDImpl * md_impl;
};

} // lpmd

#endif


