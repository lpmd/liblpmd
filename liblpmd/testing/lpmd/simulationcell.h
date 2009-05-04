//
//
//

/**
 *  \file simulationcell.h
 *  \brief Declaración de la clase SimulationCell.
 *  \author GNM
 */

#ifndef __LPMD_SIMULATIONCELL_H__
#define __LPMD_SIMULATIONCELL_H__

#include <lpmd/particleset.h>
#include <lpmd/cell.h>
#include <lpmd/neighbor.h>
#include <lpmd/error.h>

namespace lpmd
{
 class CellManager;           // forward declaration 

 class CellManagerMissing: public Error
 {
  public:
    CellManagerMissing();
 };

/**
 *  SimulationCell representa una celda de simulación, definida en una región del espacio y conteniendo un set de partículas.
 */
class SimulationCell: public ParticleSet, public BasicCell
{
 public:
   // Constructores y destructor

   SimulationCell();
   SimulationCell(int na, int nb, int nc, bool pa=true, bool pb=true, bool pc=true);
   SimulationCell(const SimulationCell & sc);                                         // Copy constructor
   virtual ~SimulationCell();

   SimulationCell & operator=(const SimulationCell & sc);

   // Metodos
   Cell & GetCell() const; // FIXME: que nombre ponerle?
   void SetCell(const Cell & c);

   void SetPart(const ParticleSet & p);

   // Metodos que combinan las facilidades de ParticleSet y Cell

   Vector FracPosition(long i) const;
   void SetPosition(long i, const Vector & p);                 // Setea la posicion real, corrigiendo si la particula sale de la caja
   void SetVelocity(long i, const Vector & v);                 // Setea la velocidad, sin cambios
   void SetAcceleration(long i, const Vector & ac);            // Setea la aceleracion, sin cambios
   void SetFracPosition(long i, const Vector & fp);            // Setea la posicion real (dentro de la caja) pero recibiendo un argumento fraccionario
   void SetColor(long i, const Vector & c);
   void SetColor(long i, const double & c);

   void ClearForces();

   double Distance(long i, long j);
   double RealDistance(long i, long j);
   double DistanceToReplica(long i, long j, long nx, long ny, long nz);
   double Distance2(long i,long j);

   CellManager & GetCellManager() const;
   void SetCellManager(CellManager & cm);
   double CMCutoff() const;

   double Angle(long i, long j, long k);
   Vector VectorDistance(long i, long j); 
   Vector VectorRealDistance(long i, long j);
   Vector VectorDistanceToReplica(long i, long j, long nx, long ny, long nz);

   const bool & Periodicity(int q) const;
   bool & Periodicity(int q);
   Vector ScaleByCell(const BasicVector & cv) const;
   void ConvertToExternal(BasicVector & v) const;
   void ConvertToInternal(BasicVector & v) const;
   Vector Displacement(const Vector & a, const Vector & b) const;
   double Volume() const;

   void BuildNeighborList(long i, std::vector<Neighbor> & nlist, bool full, double rcut);

   void RealPos();
   void FracPos();
   void Center();
   void UnCenter();

   // 
   void Rescale(double f); 
   void Rescale(double f, int i);
   void RescalePercent(double p);
   void RescalePercent(double p, int i); 
   void RescaleVector(Vector sx, Vector sy, Vector sz);
   void RescaleVector(Vector s, int i);

   // Propiedades fisicas
   double Pressure() const;
   double Density() const;
   double ParticleDensity() const;

   //set de Correccion al virial
   void AddToVirial(double vir);
   double Virial() const; 

   ParamList & MetaData() const;

   //set para asingacion de stress
   double& StressTensor(int alpha, int beta);

 private:
   class SimCellImpl * impl;
};

} // lpmd

#endif


