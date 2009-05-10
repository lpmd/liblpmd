/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/pluginmanager.h>
#include <lpmd/simulationcell.h>
#include <lpmd/timer.h>
#include <lpmd/simulation.h>
#include <lpmd/cellgenerator.h>
#include <lpmd/cellmanager.h>
#include <lpmd/integrator.h>
#include <lpmd/potentialarray.h>

#include <iostream>

using namespace lpmd;

#define INTEGRATOR "euler"

int main()
{
 Simulation md;                                            // define md como un objeto de dinamica molecular
 PluginManager pm;                                 // define pm como un manejador de plugins

 SimulationCell cell;   // cell es la celda de simulacion
 cell.GetCell()[0] = 17.1191*e1;                   // define los vectores de la celda
 cell.GetCell()[1] = 17.1191*e2;
 cell.GetCell()[2] = 17.1191*e3;
 md.SetCell(cell);                                 // asigna la celda de simulacion al objeto Simulation 

 // Carga de plugins con sus parametros
 pm.LoadPlugin("minimumimage", "cutoff 8.5");
 pm.LoadPlugin("crystalfcc", "symbol Ar nx 3 ny 3 nz 3");
 pm.LoadPlugin("lennardjones", "sigma 3.41 epsilon 0.0103408 cutoff 8.5");
 pm.LoadPlugin(INTEGRATOR, "dt 1.0");

 CellManager & cm = CastModule<CellManager>(pm["minimumimage"]);
 cell.SetCellManager(cm);                                        // asigna el manejador de celda

 CellGenerator & cg = CastModule<CellGenerator>(pm["crystalfcc"]);
 cg.Generate(cell);

 Potential & pot = CastModule<Potential>(pm["lennardjones"]);
 PotentialArray & potarray = md.GetPotentialArray();
 potarray.Set("Ar", "Ar", pot);                            // asigna el potencial lennardjones al arreglo de potenciales de Simulation

 Integrator & integ = CastModule<Integrator>(pm[INTEGRATOR]);
 md.SetIntegrator(integ);

 cell.InitVelocities();
 cell.SetTemperature(168.0);

 potarray.UpdateForces(cell);

 md.Initialize(); 
 md.DoStep();

 double av=0.0, av2=0.0;
 long nsteps = 5000, nav = 0;
 Timer timer;
 timer.Start();
 for (long i=0;i<nsteps;++i)
 {
  md.DoStep();
  if (i % 100 == 0)
  {
   double kin_en = cell.KineticEnergy();
   double pot_en = potarray.energy(cell); 
   double tot_en = kin_en + pot_en;
   double temp = cell.Temperature(); 
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

