//
//
//

#ifndef __LPMD_SIMULATIONCELL_H__
#define __LPMD_SIMULATIONCELL_H__

#include <lpmd/vector.h>
#include <lpmd/atom.h>
#include <lpmd/cell.h>
#include <lpmd/cellmanager.h>
#include <lpmd/error.h>
#include <lpmd/session.h>
#include <lpmd/util.h>

// FIXME: no corresponde aqui
const double kboltzmann = 8.6173422E-05;
const double kin2ev = 103.64269;

namespace lpmd
{

 class CellManagerMissing: public Error
 {
  public:
    CellManagerMissing(): Error("No CellManager has been defined") { }
 };


class SimulationCell: public std::vector<Atom>
{
 public:
   SimulationCell(): std::vector<Atom>(), innercm(0), virial(0) { }
   SimulationCell(long int s, const Atom & at): std::vector<Atom>(s, at), innercm(0), virial(0) { }

   Cell & GetCell() { return innercell; }

   void SetPosition(long i, const BasicVector & p) { (*this)[i].Position() = innercell.FittedInside(p); }

   void SetFracPosition(long i, const BasicVector & fp) { (*this)[i].Position() = innercell.ScaleByCell(fp); }

   void InitVelocities()
   {
    Vector vel, totalp;
    long nparts = size();
    double targettemp = 1.0;
    double Kpart = (3.0/2.0)*kboltzmann*targettemp;
    for (int i=0;i<nparts;++i)
    {
     double v2 = 2.0*Kpart/(*this)[i].Mass();
     vel = RandomVector(sqrt(v2));
     totalp += vel*(*this)[i].Mass();
     (*this)[i].Velocity() = vel;
    }
    totalp = totalp/nparts;
    for (int i=0;i<nparts;++i)
    {
     double mass = (*this)[i].Mass();
     (*this)[i].Velocity() -= (totalp/mass);
    }
   }

   void SetTemperature(double temp, double dt=1.0, double tau=1.0)
   {
    Vector vel;
    double xi, ti = Temperature();
    for (unsigned long int i=0;i<size();++i)
    {
     xi = sqrt(1.0 + (double(dt)/tau)*(temp/ti - 1.0));
     (*this)[i].Velocity() *= xi;
    }
   }

   double KineticEnergy() const 
   {
    double K = 0.0;
    for (unsigned long int i=0;i<size();++i) K += 0.5*(*this)[i].Mass()*(*this)[i].Velocity().SquareModule();
    return K*kin2ev;
   }

   double Temperature() const
   {
    if (size() == 0) EndWithError("Cannot compute temperature without atoms!");
    double K = KineticEnergy();
    return (2.0/3.0)*K/(kboltzmann*double(size()));
   }

   // Metodos problematicos

   Atom & Create(Atom * at) { push_back(*at); return (*at); }

   void SetCellManager(CellManager & cm) 
   { 
    cm.Reset();
    innercm = &cm; 
    cm.UpdateCell(*this);
   }

   CellManager & GetCellManager() const 
   { 
    if (innercm == 0) throw CellManagerMissing();
    return (*innercm); 
   }
   
   void BuildNeighborList(long i, std::vector<Neighbor> & nlist, bool full, double rcut)
   {
    innercm->BuildNeighborList(*this, i, nlist, full, rcut);
   }

   void AddToVirial(double vir) 
   {
    virial += vir;
   }

   double Virial() const 
   { 
    return virial; 
   }  // necesario para testear AddToVirial

   double & StressTensor(int alpha, int beta) 
   { 
    return s[alpha][beta]; 
   }

   void ClearForces() 
   {
    virial = 0;
    for (unsigned long int i=0;i<size();++i) (*this)[i].Acceleration() = Vector(0, 0, 0);
    if (innercm != 0) innercm->UpdateCell(*this);
   }

   void SetVelocity(long i, const BasicVector & v) { (*this)[i].Velocity() = v; }
   void SetAcceleration(long i, const BasicVector & a) { (*this)[i].Acceleration() = a; }
   
   Vector VectorDistance(long i, long j) { return innercell.Displacement((*this)[i].Position(), (*this)[j].Position()); }

  Cell innercell;
  CellManager * innercm;
  double virial;
  double s[3][3]; 
};

} // lpmd

#endif

