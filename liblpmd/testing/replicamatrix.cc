/*
 *
 *
 */

#include "distcache.h"

ReplicaMatrix::ReplicaMatrix(long nrx, long nry, long nrz)
{
 matrix = NULL;
 SetNumberOfReplicas(nrx, nry, nrz);
}

ReplicaMatrix::ReplicaMatrix()
{
 matrix = NULL;
}

ReplicaMatrix::~ReplicaMatrix()
{
 for (long j=0;j<m_nry;++j)
    for (long i=0;i<m_nrx;++i) delete [] matrix[i][j];
 for (long j=0;j<m_nry;++j) delete [] matrix[j];
 delete [] matrix;
}

void ReplicaMatrix::SetNumberOfReplicas(long nrx, long nry, long nrz)
{
 if (matrix == NULL)
 {
  m_nrx = nrx;
  m_nry = nry;
  m_nrz = nrz;
  matrix = new DistanceInfo**[nrx];
  for (long i=0;i<nrx;++i) 
  {
   matrix[i] = new DistanceInfo*[nry];
   for (long j=0;j<nry;++j) 
   {
    matrix[i][j] = new DistanceInfo[nrz];
    for (long k=0;k<nrz;++k)
    {
     matrix[i][j][k].active = false;
     matrix[i][j][k].r = 0.0;
     matrix[i][j][k].r2 = 0.0;
     matrix[i][j][k].d[0] = 0.0e0;
     matrix[i][j][k].d[1] = 0.0e0;
     matrix[i][j][k].d[2] = 0.0e0;
    }
   }
  }
  bx = ((m_nrx-1)/2);
  by = ((m_nry-1)/2);
  bz = ((m_nrz-1)/2);
 }
}

void ReplicaMatrix::SetActive(bool t) 
{ 
 long cc = 0;
 for (long i=0;i<m_nrx;++i)
  for (long j=0;j<m_nry;++j) 
   for (long k=0;k<m_nrz;++k)
   {
    if (matrix[i][j][k].active == true) cc++;
    matrix[i][j][k].active = t;
   }
}

void ReplicaMatrix::SetReplica(long nx, long ny, long nz, const DistanceInfo & di)
{
 long i, j, k;
 i = bx + nx;
 j = by + ny;
 k = bz + nz;
 DistanceInfo * m = &matrix[i][j][k];
 *m = di;
 m->active = true;
}

DistanceInfo & ReplicaMatrix::GetInfo(long nx, long ny, long nz)
{
 long i, j, k;
 i = bx + nx;
 j = by + ny;
 k = bz + nz;
 return matrix[i][j][k];
}

DistanceInfo & ReplicaMatrix::GetReplica(long nx, long ny, long nz)
{
 DistanceInfo & di = GetInfo(nx, ny, nz);
 if (di.active) return di;
 return wrongDistance;
}


