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

namespace lpmd
{

 class CellManagerMissing: public Error
 {
  public:
    CellManagerMissing(): Error("No CellManager has been defined") { }
 };

 class SimulationCell: public std::vector<Atom*>
 {
  public:
   SimulationCell(int nx, int ny, int nz, bool px, bool py, bool pz): innercm(0) 
   {
    virial=0;
    c.Periodicity(0) = px;
    c.Periodicity(1) = py;
    c.Periodicity(2) = pz;
   }

   inline Atom const & operator[](long int i) const { return *(std::vector<Atom*>::operator[](i)); }
   inline Atom & operator[](long int i) { return *(std::vector<Atom*>::operator[](i)); }

   Atom & Create(Atom * at) { push_back(at); return *at; } 

   void SetPosition(long i, const Vector & p) { (*this)[i].Position() = c.FittedInside(p); }

   void SetFracPosition(long i, const Vector & fp) 
   {
    (*this)[i].Position() = c.ScaleByCell(fp);
   }

   void SetVelocity(long i, const Vector & v) { (*this)[i].Velocity() = v; }

   void SetAcceleration(long i, const Vector & a) { (*this)[i].Acceleration() = a; }

   void ClearForces() 
   {
    virial=0;
    for (unsigned long int i=0;i<size();++i) (*this)[i].Acceleration() = Vector(0, 0, 0);
   }

   void SetCellManager(CellManager & cm) { innercm = &cm; }

   CellManager & GetCellManager() const 
   { 
    if (innercm == 0) throw CellManagerMissing();
    return (*innercm); 
   }

   void BuildNeighborList(long i, std::vector<Neighbor> & nlist, bool full, double rcut)
   {
    innercm->BuildNeighborList(*this, i, nlist, full, rcut);
   }

   Vector VectorDistance(long i, long j) { return c.Displacement((*this)[i].Position(), (*this)[j].Position()); }

   void AddToVirial(double vir) {virial += vir;}

   double Virial() const { return virial; }  // necesario para testear AddToVirial

   double & StressTensor(int alpha, int beta) { return s[alpha][beta]; }

   Cell & GetCell() { return c; }

   void InitVelocities() 
   {
    Vector vel, totalp;
    long nparts = size();
    for (int i=0;i<nparts;++i)
    {
     vel = RandomVector(1.0);
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

   double Temperature() const 
   {
    double K = 0.0;
    for (unsigned long int i=0;i<size();++i) K += 0.5*(*this)[i].Mass()*(*this)[i].Velocity().SquareModule();
    return (2.0/3.0)*K/(GlobalSession.GetDouble("kboltzmann")*double(size())); 
   }

  private:
    CellManager * innercm;  // para guardar la referencia al CellManager
    double s[3][3];         // la componente del tensor de estres falsa
    Cell c;                 // la Cell interna
    double virial;          //valor del virial
 };

} // lpmd

#endif


