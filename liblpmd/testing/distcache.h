/*
 *
 *
 *
 */

#ifndef __LPMD_DISTCACHE_H__
#define __LPMD_DISTCACHE_H__

#include <lpmd/vector.h>

//
//
//

namespace lpmd
{
 class SimulationCell;        // forward declaration
}

class DistanceInfo
{
 public:
   bool active;
   lpmd::Vector d;
   double r;
   double r2;

   DistanceInfo() { active = false; }
};

//
//
//

class ReplicaMatrix
{
 public:
   ReplicaMatrix();
   ReplicaMatrix(long nrx, long nry, long nrz);
   ~ReplicaMatrix();

   void SetNumberOfReplicas(long nrx, long nry, long nrz);

   void SetReplica(long nx, long ny, long nz, const DistanceInfo & di);
   DistanceInfo & GetReplica(long nx, long ny, long nz);
   void SetActive(bool t);

 private:
   DistanceInfo & GetInfo(long nx, long ny, long nz);

   long m_nrx, m_nry, m_nrz;
   long bx, by, bz;
   DistanceInfo *** matrix;
   DistanceInfo wrongDistance;
};

//
//
//

class DistanceCache
{
 public:
   //
   DistanceCache(const lpmd::SimulationCell & sc);
   ~DistanceCache();

   void Clear();
   const lpmd::Vector VectorDistance(long i, long j);
   double Distance(long i, long j);
   double Distance2(long i, long j);
   const lpmd::Vector VectorDistanceToReplica(long i, long j, long nx, long ny, long nz);

   void Add(const DistanceInfo & di);

 private:
   //
   long n;
   const lpmd::SimulationCell * parent;
   DistanceInfo ** distcache;
   ReplicaMatrix ** dcreplica;
};

#endif


