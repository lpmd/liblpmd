//
//
//

#ifndef __LPMD_SIMULATIONCELL_H__
#define __LPMD_SIMULATIONCELL_H__

#include <lpmd/vector.h>
#include <lpmd/atom.h>
#include <lpmd/cell.h>
#include <lpmd/cellmanager.h>

namespace lpmd
{

 class SimulationCell
 {
  public:
   SimulationCell(int nx, int ny, int nz, bool px, bool py, bool pz) { }

   inline unsigned long int size() const { return 0; }

   Atom const & operator[](long int i) const { return at; }
   Atom & operator[](long int i) { return at; }

   Atom & Create(Atom * at) { return (*at); } 

   void SetPosition(long i, const Vector & p) { }
   void SetFracPosition(long i, const Vector & fp) { }
   void SetVelocity(long i, const Vector & v) { }
   void SetAcceleration(long i, const Vector & a) { }

   void ClearForces() { }

   void SetCellManager(CellManager & cm) { innercm = &cm; }
   CellManager & GetCellManager() const { return (*innercm); }

   void BuildNeighborList(long i, std::vector<Neighbor> & nlist, bool full, double rcut)
   {
    innercm->BuildNeighborList(*this, i, nlist, full, rcut);
   }

   Vector VectorDistance(long i, long j) { return Vector(); }

   void AddToVirial(double vir) { }
   double & StressTensor(int alpha, int beta) { return s; }

   Cell & GetCell() { return c; }

   void InitVelocities() { }
   void SetTemperature(double temp, double dt=1.0, double tau=1.0) { }
   double Temperature() const { return 0.0; }

  private:
    Atom at;                // atomo temporal para poder retornar algo en operator[]
    CellManager * innercm;  // para guardar la referencia al CellManager
    double s;               // la componente del tensor de estres falsa
    Cell c;                 // la Cell interna
 };

} // lpmd

#endif


