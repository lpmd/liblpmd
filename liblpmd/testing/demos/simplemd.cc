/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/array.h>
#include <lpmd/pluginmanager.h>
#include <lpmd/timer.h>
#include <lpmd/simulationbuilder.h>
#include <lpmd/properties.h>
#include <lpmd/manipulations.h>
#include <lpmd/cellgenerator.h>
#include <lpmd/cellmanager.h>
#include <lpmd/integrator.h>
#include <lpmd/potential.h>

#include <iostream>

#define NX 3
#define NY 3
#define NZ 3

using namespace lpmd;

#define INTEGRATOR "euler"

template <typename AtomContainer, typename CellType> void CheckAllInside(AtomContainer & atoms, CellType & cell)
{
 for (int q=0;q<atoms.Size();++q)
 {
  Vector vpos = cell.Fractional(atoms[q].Position());
  assert((vpos[0] >= 0.0) && (vpos[0] <= 1.0));
  assert((vpos[1] >= 0.0) && (vpos[1] <= 1.0));
  assert((vpos[2] >= 0.0) && (vpos[2] <= 1.0));
 }
}

int main()
{
 Simulation & md = SimulationBuilder::CreateFixedOrthogonal(NX*NY*NZ*4, Atom("Ar"));

 BasicCell & cell = md.Cell();
 cell[0] = NX*5.7063666667*e1;
 cell[1] = NX*5.7063666667*e2;
 cell[2] = NX*5.7063666667*e3;

 // Carga de plugins con sus parametros
 PluginManager pm;

 BasicParticleSet & atoms = md.Atoms();
 assert(fabs(atoms[0].Mass() - 39.948) < 1.0E-10);

 CellGenerator & cg = pm.LoadPluginAs<CellGenerator>("crystal3d", "type fcc symbol Ar nx "+ToString(NX)+" ny "+ToString(NY)+" nz "+ToString(NZ));
 cg.Generate(md);
 
 CenterByCenterOfMass(atoms, cell);
 CheckAllInside(atoms, cell);

 assert(atoms.Size() == NX*NY*NZ*4);
 std::cerr << "-> Running MD with " << atoms.Size() << " atoms\n";

 //md.SetCellManager(pm.LoadPluginAs<CellManager>("minimumimage", "cutoff 8.5"));
 //md.SetCellManager(pm.LoadPluginAs<CellManager>("lc2", "cutoff 8.5 nx 15 ny 15 nz 15"));
 md.SetCellManager(pm.LoadPluginAs<CellManager>("linkedcell", "cutoff 8.5 nx 14 ny 14 nz 14"));

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

 return 0;
}

