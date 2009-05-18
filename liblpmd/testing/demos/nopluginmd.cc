/*
 * Ejemplo simple de dinamica molecular usando solo Simulation
 */

#include <lpmd/simulation.h>
#include <lpmd/timer.h>
#include <lpmd/properties.h>
#include <lpmd/fixedsizeparticleset.h>
#include <lpmd/orthogonalcell.h>

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

double TestUpdateForces(FixedSizeParticleSet & atoms, OrthogonalCell & cell)
{
 double r, energycache = 0.0;
 Vector dist;
 for (long i=0;i<atoms.Size()-1;++i)
  for (long j=i+1;j<atoms.Size();++j)
  {
   const Vector & v_i = atoms[i].Position();
   const Vector & v_j = atoms[j].Position();
   Vector dist = cell.Displacement(v_i, v_j);
   r = dist.Module();
   if (r < 8.5)
   {
    energycache += pairEnergy(r);
    const Vector ff = pairForce(dist);
    atoms[i].Acceleration() += ff*(forcefactor/atoms[i].Mass());
    atoms[j].Acceleration() -= ff*(forcefactor/atoms[j].Mass());
   }
  }
 return energycache;
}

void TestUpdatePositions(FixedSizeParticleSet & atoms, OrthogonalCell & cell, double dt)
{
 for (long int i=0;i<atoms.Size();++i)
 {
  const Atom & now = atoms[i];
  atoms[i].Position() = cell.FittedInside(now.Position() + now.Velocity()*dt);
  atoms[i].Velocity() += now.Acceleration()*dt;
 }
}

double TestDoStep(FixedSizeParticleSet & atoms, OrthogonalCell & cell, double dt)
{
 for (unsigned long int i=0;i<atoms.Size();++i)
    atoms[i].Acceleration() = Vector(0.0, 0.0, 0.0);
 double e = TestUpdateForces(atoms, cell);
 TestUpdatePositions(atoms, cell, dt);
 return e;
}

template <typename AtomContainer> void CheckMasses(AtomContainer & atoms)
{
 for (int i=0;i<108;++i)
    assert(fabs(atoms[i].Mass()-39.948) < 1.0E-08);
}

template <typename AtomContainer> void CheckAllInside(AtomContainer & atoms)
{
 for (int q=0;q<108;++q)
 {
  assert((atoms[q].Position()[0] >= 0.0) && (atoms[q].Position()[0] < 17.1191));
  assert((atoms[q].Position()[1] >= 0.0) && (atoms[q].Position()[1] < 17.1191));
  assert((atoms[q].Position()[2] >= 0.0) && (atoms[q].Position()[2] < 17.1191));
 }
}

template <typename AtomContainer, typename CellType> void GenerateFCC(AtomContainer & atoms, CellType & cell)
{
 long int cc = 0;
 double ax = 1.0/3.0;
 double ay = 1.0/3.0;
 double az = 1.0/3.0;
 for (long k=0;k<3;++k)
  for (long j=0;j<3;++j)
   for (long i=0;i<3;++i)
   {
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+0.51)*ax, (double(j)+0.5)*ay, (double(k)+0.5)*az));
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+0.51)*ax, double(j)*ay, (double(k)+1.0)*az));
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+1.01)*ax, double(j)*ay, (double(k)+0.5)*az));
    atoms[cc++].Position() = cell.ScaleByCell(Vector((double(i)+1.01)*ax, (double(j)+0.5)*ay, (double(k)+1.0)*az));
   }
}


int main()
{
 Simulation * simp = FixedOrthogonalEngine(108, Atom("Ar"));
 Simulation & md = (*simp);

 FixedSizeParticleSet & atoms = dynamic_cast<FixedSizeParticleSet&> (md.Atoms());
 OrthogonalCell & cell = dynamic_cast<OrthogonalCell &> (md.Cell());

 cell[0] = 1*17.1191*e1;                   // define los vectores de la celda
 cell[1] = 1*17.1191*e2;
 cell[2] = 1*17.1191*e3;

 //CheckMasses(atoms);

 GenerateFCC(atoms, cell);
 
 md.SetTemperature(168.0);

 TestDoStep(atoms, cell, DT);

 double av=0.0, av2=0.0;
 long nsteps = 5000, nav = 0;
 Timer timer;
 timer.Start();
 for (long i=0;i<nsteps;++i)
 {
  double pot_en = TestDoStep(atoms, cell, DT);
  //CheckAllInside(atoms);
  if (i % 100 == 0)
  {
   double kin_en = KineticEnergy(atoms);
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

