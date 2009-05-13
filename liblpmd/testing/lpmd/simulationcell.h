//
//
//

#ifndef __LPMD_SIMULATIONCELL_H__
#define __LPMD_SIMULATIONCELL_H__

#include <lpmd/particleset.h>
#include <lpmd/cell.h>
#include <lpmd/cellmanager.h>
#include <lpmd/error.h>
#include <lpmd/session.h>
#include <lpmd/util.h>

#include <lpmd/properties.h>

// FIXME: no corresponde aqui
const double kboltzmann = 8.6173422E-05;

namespace lpmd
{

 class CellManagerMissing: public Error
 {
  public:
    CellManagerMissing(): Error("No CellManager has been defined") { }
 };


class SimulationCell: public ParticleSet
{
 public:
   SimulationCell(): ParticleSet(), managedcell(true), innercm(0), virial(0) 
   {
    innercell = new Cell();
   }

   SimulationCell(long int s, const Atom & at): ParticleSet(s, at), managedcell(true), innercm(0), virial(0) 
   {
    innercell = new Cell(); 
   }

   virtual ~SimulationCell() { if (managedcell) delete innercell; }

   void SetCell(BasicCell & c) { innercell = &c; managedcell = false; }
   BasicCell & GetCell() { return *innercell; }

   void SetPosition(long i, const Vector & p) { (*this)[i].Position() = innercell->FittedInside(p); }

   void SetFracPosition(long i, const Vector & fp) { (*this)[i].Position() = innercell->ScaleByCell(fp); }

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

   void AddToVirial(double vir) { virial += vir; }

   double Virial() const { return virial; }

   double & StressTensor(int alpha, int beta) { return s[alpha][beta]; }

   void ClearForces() 
   {
    virial = 0;
    for (unsigned long int i=0;i<Size();++i) (*this)[i].Acceleration() = Vector(0, 0, 0);
    if (innercm != 0) innercm->UpdateCell(*this);
   }

   Vector VectorDistance(long i, long j) { return innercell->Displacement((*this)[i].Position(), (*this)[j].Position()); }

  BasicCell * innercell;
  bool managedcell;
  CellManager * innercm;
  double virial;
  double s[3][3]; 
};

} // lpmd

#endif

