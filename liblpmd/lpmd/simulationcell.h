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

#include "particles.h"
#include "cell.h"

namespace lpmd
{

class DistCacheItem
{
 public:
   bool active;
   Vector d;
   double r;
   double r2;
};

/**
 *  SimulationCell representa una celda de simulación, definida en una región del espacio y conteniendo un set de partículas.
 */
class SimulationCell: public Particles, public Cell
{
 public:
   // Constructores y destructor
   SimulationCell();
   SimulationCell(int na, int nb, int nc, bool pa=true, bool pb=true, bool pc=true);
   virtual ~SimulationCell();

   // Metodos
   void SetCell(const Cell & c);
   void SetPart(const Particles & p);

   // Metodos que combinan las facilidades de Particles y Cell
   // Vector Position(long i) const;                              // Da la posicion real, i.e., escalada por los largos de celda
   Vector FracPosition(long i) const;
   void SetPosition(long i, const Vector & p);                 // Setea la posicion real, corrigiendo si la particula sale de la caja
   void SetVelocity(long i, const Vector & v);                 // Setea la velocidad, sin cambios
   void SetAcceleration(long i, const Vector & ac);            // Setea la aceleracion, sin cambios

   void SetFracPosition(long i, const Vector & fp);            // Setea la posicion real (dentro de la caja) pero recibiendo un argumento fraccionario

   void UseDistanceCache(bool activ);
   void ClearDistanceCache();
   void ClearForces();

   double Distance(long i, long j);
   double RealDistance(long i, long j);
   double DistanceToReplica(long i, long j, long nx, long ny, long nz) const;
   double Distance2(long i,long j);
   double Angle(long i, long j, long k);
   Vector Displacement(const Vector & a, const Vector & b) const;
   Vector VectorDistance(long i, long j); 
   Vector VectorRealDistance(long i, long j);
   Vector VectorDistanceToReplica(long i, long j, long nx, long ny, long nz) const;
   void RealPos();
   void FracPos();
   void SuperCell();                                              //Re-Genera una supercelda a partir de la opcion repeat dada en input;
   void Center();
   void UnCenter();

   // 
   void Rescale(double f); 
   void Rescale(double f,int i);
   void RescalePercent(double p);
   void RescalePercent(double p, int i); 

   // Propiedades fisicas
   double Pressure() const;

 private:
   bool pa,pb,pc;
   unsigned int na,nb,nc;
   bool use_dc;
   DistCacheItem ** distcache;
};

} // lpmd

#endif


