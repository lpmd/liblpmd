/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/array.h>
#include <lpmd/pluginmanager.h>
#include <lpmd/timer.h>
#include <lpmd/simulation.h>
#include <lpmd/properties.h>
#include <lpmd/cellgenerator.h>
#include <lpmd/cellmanager.h>
#include <lpmd/integrator.h>
#include <lpmd/potential.h>

#include <iostream>

using namespace lpmd;

#define INTEGRATOR "euler"

int main()
{
 Simulation * simp = FixedOrthogonalEngine(108, Atom("Ar"));
 Simulation & md = (*simp);

 BasicCell & cell = md.Cell();
 cell[0] = 17.1191*e1;
 cell[1] = 17.1191*e2;
 cell[2] = 17.1191*e3;

 // Carga de plugins con sus parametros
 PluginManager pm;

 md.SetCellManager(pm.LoadPluginAs<CellManager>("minimumimage", "cutoff 8.5"));

 BasicParticleSet & atoms = md.Atoms();
 assert(fabs(atoms[0].Mass() - 39.948) < 1.0E-10);

 CellGenerator & cg = pm.LoadPluginAs<CellGenerator>("crystalfcc", "symbol Ar nx 3 ny 3 nz 3");
 cg.Generate(md);

 assert(atoms.Size() == 108);

 Potential & pot = pm.LoadPluginAs<Potential>("lennardjones", "sigma 3.41 epsilon 0.0103408 cutoff 8.5");
 Array<Potential &> & potentials = md.Potentials();

 pot.SetValidSpecies(18, 18);
 potentials.Append(pot);

 md.SetTemperature(168.0);
 md.SetIntegrator(pm.LoadPluginAs<Integrator>(INTEGRATOR, "dt 1.0"));

 for (int i=0;i<atoms.Size();++i) atoms[i].Acceleration() = Vector(0.0, 0.0, 0.0);
 potentials[0].Initialize(md);
 potentials[0].UpdateForces(md);
 
 assert(potentials.Size() == 1);
 assert(&(potentials[0]) == &pot);

 md.DoStep();
 double av=0.0, av2=0.0;
 long nsteps = 5000, nav = 0;
 Timer timer;
 timer.Start();
 for (long i=0;i<nsteps;++i)
 {
  for (int k=0;k<atoms.Size();++k) atoms[k].Acceleration() = Vector(0.0, 0.0, 0.0);
  md.DoStep();
  if (i % 100 == 0)
  {
   double kin_en = KineticEnergy(atoms), pot_en = 0.0;
   for (int q=0;q<potentials.Size();++q) { pot_en += potentials[q].energy(md); }
   double tot_en = kin_en + pot_en;
   double temp = Temperature(atoms);
   std::cout << i << "  " << pot_en << "  " << kin_en << "  " << tot_en << "  " << temp << '\n';
   av += tot_en;
   av2 += (tot_en*tot_en);
   nav++;
  }
 }
 timer.Stop();
 double totalenergy_av = av/double(nav);
 double totalenergy_fluct = sqrt(av2/double(nav)-pow(av/double(nav), 2.0));
 std::cout << "Simulation over " << nsteps << " steps\n";
 std::cout << "Total energy average = " << totalenergy_av << '\n';
 std::cout << "Total energy fluctuations = " << totalenergy_fluct << '\n';
 std::cout << "Fluctuation percentage = " << 100.0*totalenergy_fluct/fabs(totalenergy_av) << "%\n";
 timer.ShowElapsedTimes();

 delete simp;
 return 0;
}

