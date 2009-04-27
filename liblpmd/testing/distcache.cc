/*
 *
 *
 *
 */

#include "distcache.h"

#include <lpmd/simulationcell.h>

DistanceCache::DistanceCache(const SimulationCell & sc)
{ 
 parent = &sc;
 n = parent->size();
 distcache = new DistanceInfo*[n];
 dcreplica = new ReplicaMatrix*[n];
 for (long i=0;i<n;++i) 
 {
  distcache[i] = new DistanceInfo[n];
  dcreplica[i] = new ReplicaMatrix[n];
 }
 for (long i=0;i<n;++i)
   for (long j=0;j<n;++j) dcreplica[i][j].SetNumberOfReplicas(3, 3, 3);
 Clear();
}

DistanceCache::~DistanceCache()
{
 Clear();
 for (long i=0;i<n;++i) 
 {
  delete [] distcache[i];
  delete [] dcreplica[i];
 }
 delete [] distcache;
 delete [] dcreplica;
}

void DistanceCache::Clear()
{
 for (long i=0;i<n;++i) 
   for (long j=0;j<n;++j) 
   {
    distcache[i][j].active = false;
    dcreplica[i][j].SetActive(false);
   }
}


const lpmd::Vector DistanceCache::VectorDistance(long i, long j)
{
 // Consulta al cache de distancias
 DistanceInfo & p = distcache[j][i];
 if (p.active == true) return p.d;
 DistanceInfo & pinv = distcache[i][j];
 if (p.active == true) return pinv.d*(-1.0);
 else
 {
  const ParticleSet & set = (*parent);
  const Vector & vd = parent->Displacement(set[i].Position(), set[j].Position());
  double r = vd.Module();
  double r2 = r*r;
  //
  distcache[j][i].active = true;
  distcache[j][i].d = vd;
  distcache[j][i].r = r;
  distcache[j][i].r2 = r2;
  //
  distcache[i][j].active = true;
  distcache[i][j].d = vd*(-1.0);
  distcache[i][j].r = r;
  distcache[i][j].r2 = r2;
  return vd;
 }
}

double DistanceCache::Distance(long i, long j)
{
 // Consulta al cache de distancias
 DistanceInfo & p = distcache[j][i];
 if (p.active == true) return p.r;
 DistanceInfo & pinv = distcache[i][j];
 if (p.active == true) return pinv.r;
 else
 {
  const ParticleSet & set = (*parent);
  const Vector & vd = parent->Displacement(set[i].Position(), set[j].Position());
  double r = vd.Module();
  double r2 = r*r;
  //
  distcache[j][i].active = true;
  distcache[j][i].d = vd;
  distcache[j][i].r = r;
  distcache[j][i].r2 = r2;
  //
  distcache[i][j].active = true;
  distcache[i][j].d = vd*(-1.0);
  distcache[i][j].r = r;
  distcache[i][j].r2 = r2;
  return r;
 }
}

double DistanceCache::Distance2(long i, long j)
{
 // Consulta al cache de distancias
 DistanceInfo & p = distcache[j][i];
 if (p.active == true) return p.r2;
 else
 {
  const ParticleSet & set = (*parent);
  const Vector & vd = parent->Displacement(set[i].Position(), set[j].Position());
  distcache[j][i].active = true;
  distcache[j][i].d = vd;
  double r = vd.Module();
  double r2 = r*r;
  distcache[j][i].r = r;
  distcache[j][i].r2 = r2;
  return r2;
 }
}

const lpmd::Vector DistanceCache::VectorDistanceToReplica(long i, long j, long nx, long ny, long nz)
{
 // Consulta al cache de distancias
 DistanceInfo di = dcreplica[j][i].GetReplica(nx, ny, nz);
 if (di.active) return di.d;
 di = dcreplica[i][j].GetReplica(nx, ny, nz);
 if (di.active) 
 {
  di.d = di.d*(-1.0);
  return di.d;
 }
 const ParticleSet & set = (*parent);
 const Vector & vi = set[i].Position();
 const Vector & vj = set[j].Position(); 
 if (nx==0 && (ny==0 && nz==0)) di.d = vj-vi;
//FIXME : ScaleBycell comentado por nuevo vector.h 
// else di.d = parent->ScaleByCell(Vector(nx, ny, nz)) + vj - vi;
 di.active = true;
 di.r = di.d.Module();
 di.r2 = di.r*di.r;
 dcreplica[j][i].SetReplica(nx, ny, nz, di);
 return di.d;
}

