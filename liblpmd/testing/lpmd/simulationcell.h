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
   SimulationCell(int nx, int ny, int nz, bool px, bool py, bool pz): innercm(0) { }

   inline Atom const & operator[](long int i) const { return *(std::vector<Atom*>::operator[](i)); }
   inline Atom & operator[](long int i) { return *(std::vector<Atom*>::operator[](i)); }

   Atom & Create(Atom * at) { push_back(at); return *at; } 

   void SetPosition(long i, const Vector & p) { }
   void SetFracPosition(long i, const Vector & fp) { }
   void SetVelocity(long i, const Vector & v) { }
   void SetAcceleration(long i, const Vector & a) { }

   void ClearForces() { }

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

   void AddToVirial(double vir) { }
   double Virial() const { return 0.0; }  // necesario para testear AddToVirial

   double & StressTensor(int alpha, int beta) { return s; }

   Cell & GetCell() { return c; }

   void InitVelocities() { }
   void SetTemperature(double temp, double dt=1.0, double tau=1.0) { }
   double Temperature() const { return 0.0; }

  private:
    CellManager * innercm;  // para guardar la referencia al CellManager
    double s;               // la componente del tensor de estres falsa
    Cell c;                 // la Cell interna
 };

} // lpmd

#endif


