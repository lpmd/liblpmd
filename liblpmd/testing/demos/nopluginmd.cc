/*
 * Ejemplo simple de dinamica molecular usando solo SimulationCell
 */

#include <lpmd/simulationcell.h>

#include <lpmd/timer.h>
#include <lpmd/atom.h>

#define DT (1.0)

const double sigma = 3.41;
const double epsilon = 0.0103408;
const double forcefactor = 0.0096485341;

using namespace lpmd;

#include <iostream>

double pairEnergy(const double & r)
{
 double rtmp=sigma/r;
 double r6 = rtmp*rtmp*rtmp*rtmp*rtmp*rtmp;
 double r12 = r6*r6;
 return 4.0e0*epsilon*(r12 - r6);
}

Vector pairForce(const Vector & r)
{
 double rr2 = r.SquareModule();
 double r6 = pow(sigma*sigma / rr2, 3.0e0);
 double r12 = r6*r6;
 double ff = -48.0e0*(epsilon/rr2)*(r12 - 0.50e0*r6);
 return r*ff;
}

double TestUpdateForces(SimulationCell & sc)
{
 double energycache = 0.0;
 for (long i=0;i<sc.size()-1;++i)
 {
  for (long j=i+1;j<sc.size();++j)
  {
   const BasicVector & v_i = sc[i].Position();
   const BasicVector & v_j = sc[j].Position();
   Vector dist = sc.GetCell().Displacement(v_i, v_j);
   if (dist.Module() < 8.5)
   {
    energycache += pairEnergy(dist.Module());
    const Vector ff = pairForce(dist);
    sc[i].Acceleration() += ff*(forcefactor/sc[i].Mass());
    sc[j].Acceleration() -= ff*(forcefactor/sc[j].Mass());
   }
  }
 }
 return energycache;
}

double TestDoStep(SimulationCell & sc, double dt)
{
 for (unsigned long int i=0;i<sc.size();++i)
    sc[i].Acceleration() = Vector(0.0, 0.0, 0.0);
 double e = TestUpdateForces(sc);
 for (unsigned long int i=0;i<sc.size();++i)
 {
  const Atom & now = sc[i];
  sc.SetPosition(i, now.Position() + now.Velocity()*dt);
  sc[i].Velocity() += now.Acceleration()*dt;
 }
 return e;
}

void GenerateFCC(SimulationCell & sc)
{
 long int cc = 0;
 double ax = 1.0/3.0;
 double ay = 1.0/3.0;
 double az = 1.0/3.0;
 for (long k=0;k<3;++k)
  for (long j=0;j<3;++j)
   for (long i=0;i<3;++i)
   {
    sc.SetFracPosition(cc++, Vector((double(i)+0.51)*ax, (double(j)+0.5)*ay, (double(k)+0.5)*az));
    sc.SetFracPosition(cc++, Vector((double(i)+0.51)*ax, double(j)*ay, (double(k)+1.0)*az));
    sc.SetFracPosition(cc++, Vector((double(i)+1.01)*ax, double(j)*ay, (double(k)+0.5)*az));
    sc.SetFracPosition(cc++, Vector((double(i)+1.01)*ax, (double(j)+0.5)*ay, (double(k)+1.0)*az));
   }
}


int main()
{
 SimulationCell cell(108, Atom("Ar"));   // cell es la celda de simulacion
 cell.GetCell()[0] = 17.1191*e1;                   // define los vectores de la celda
 cell.GetCell()[1] = 17.1191*e2;
 cell.GetCell()[2] = 17.1191*e3;

 GenerateFCC(cell);

 cell.InitVelocities();
 cell.SetTemperature(168.0);

 TestDoStep(cell, DT);

 double av=0.0, av2=0.0;
 long nsteps = 5000, nav = 0;
 Timer timer;
 timer.Start();
 for (long i=0;i<nsteps;++i)
 {
  double pot_en = TestDoStep(cell, DT);
  if (i % 100 == 0)
  {
   double kin_en = cell.KineticEnergy();
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

