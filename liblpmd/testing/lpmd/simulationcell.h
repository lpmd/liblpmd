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
   SimulationCell(int nx, int ny, int nz, bool px, bool py, bool pz): innercm(0) {virial=0;}

   inline Atom const & operator[](long int i) const { return *(std::vector<Atom*>::operator[](i)); }
   inline Atom & operator[](long int i) { return *(std::vector<Atom*>::operator[](i)); }

   Atom & Create(Atom * at) { push_back(at); return *at; } 

   void SetPosition(long i, const Vector & p) { }
   void SetFracPosition(long i, const Vector & fp) { }
   void SetVelocity(long i, const Vector & v) { }
   void SetAcceleration(long i, const Vector & a) { }

   void ClearForces() {virial=0;}

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

   Vector VectorDistance(long i, long j) { return Vector(); }

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
     Atom tmp = *((std::vector<Atom*>::operator[](i)));
     vel = Vector(2.0*drand48()-1.0, 2.0*drand48()-1.0, 2.0*drand48()-1.0);
     double mass = tmp.Mass();
     totalp = totalp + vel*mass;
     Atom a = *(std::vector<Atom*>::operator[](i));
     a.Velocity() = vel;
     *((std::vector<Atom*>::operator[](i))) = a;
    }
    totalp = totalp/nparts;
    for (int i=0;i<nparts;++i)
    {
     Atom tmp = *((std::vector<Atom*>::operator[](i)));
     vel = tmp.Velocity();
     double mass = tmp.Mass();
     vel = vel - totalp/mass;
     Atom a = tmp;
     a.Velocity() = vel;
     *((std::vector<Atom*>::operator[](i))) = a;
    }
   }

   void SetTemperature(double temp, double dt=1.0, double tau=1.0) 
   {
    Vector vel;
    double xi, ti = Temperature();
    for (unsigned long int i=0;i<size();++i)
    {
     Atom tmp = *((std::vector<Atom*>::operator[](i)));
     vel = tmp.Velocity();
     xi = sqrt(1.0 + (double(dt)/tau)*(temp/ti - 1.0));
     vel = vel*xi;
     Atom a = tmp;
     a.Velocity() = vel;
     *((std::vector<Atom*>::operator[](i))) = a;
    }
   }

   double Temperature() const 
   {
    double K = 0.0;
    Vector vel;
    for (unsigned long int i=0;i<size();++i)
    {
     Atom tmp = *((std::vector<Atom*>::operator[](i)));
     vel = tmp.Velocity();
     K += 0.5*tmp.Mass()*vel.SquareModule();
    }
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


