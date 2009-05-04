/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/api.h>
#include <iostream>

using namespace lpmd;

int main()
{
 MD md;                                            // define md como un objeto de dinamica molecular
 PluginManager pm;                                 // define pm como un manejador de plugins

 SimulationCell cell(1, 1, 1, true, true, true);   // cell es la celda de simulacion
 cell.GetCell()[0] = 17.1191*e1;                   // define los vectores de la celda
 cell.GetCell()[1] = 17.1191*e2;
 cell.GetCell()[2] = 17.1191*e3;
 md.SetCell(cell);                                 // asigna la celda de simulacion al objeto MD 

 // Carga de plugins con sus parametros
 pm.LoadPlugin("minimumimage", "cutoff 8.5");
 pm.LoadPlugin("crystalfcc", "symbol Ar nx 3 ny 3 nz 3");
 pm.LoadPlugin("lennardjones", "sigma 3.41 epsilon 0.0103408 cutoff 8.5");
 pm.LoadPlugin("beeman", "dt 1.0");
 pm.LoadPlugin("temperature", "t 168.0");
 pm.LoadPlugin("energy", "");

 CellManager & cm = CastModule<CellManager>(pm["minimumimage"]);
 cell.SetCellManager(cm);                                        // asigna el manejador de celda

 CellGenerator & cg = CastModule<CellGenerator>(pm["crystalfcc"]);
 cg.Generate(cell);

 Potential & pot = CastModule<Potential>(pm["lennardjones"]);
 PotentialArray & potarray = md.GetPotentialArray();
 potarray.Set("Ar", "Ar", pot);                            // asigna el potencial lennardjones al arreglo de potenciales de MD

 Integrator & integ = CastModule<Integrator>(pm["beeman"]);
 md.SetIntegrator(integ);

 InstantProperty & energ = CastModule<InstantProperty>(pm["energy"]);
 
 SystemModifier & therm = CastModule<SystemModifier>(pm["temperature"]);
 therm.Apply(cell);                        // aplica el termalizador "temperature" a la celda de simulacion

 md.Initialize(); 
 double av=0.0, av2=0.0;
 long nsteps = 5000, nav = 0;
 Timer timer;
 timer.Start();
 for (long i=0;i<nsteps;++i)
 {
  md.DoStep();
  if (i % 10 == 0) 
  { 
   energ.Evaluate(cell, pot);
   double tot_en = pm["energy"].GetProperty("total-energy");
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

