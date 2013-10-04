/*
 *
 *
 *
 */

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>

#include <lpmd/atomset.h>

using namespace memory;
using namespace lpmd;

static void GetBoundaryShift(const CellTopology & top, int k, double * shift)
{
 const double * cell = top.GetCellDimensions(); 
 switch(k)
 {
  case 0:
       if (top.PositionAlong(0) == top.RanksAlong(0)-1) shift[0] = cell[0];
       break;
  case 1:
       if (top.PositionAlong(0) == 0) shift[0] = -cell[0];
       break;
  case 2:
       if (top.PositionAlong(1) == top.RanksAlong(1)-1) shift[1] = cell[1];
       break;
  case 3:
       if (top.PositionAlong(1) == 0) shift[1] = -cell[1];
       break;
 };
}

static void ScanForBoundaries(const CellTopology & top, double cutoff, double * pos, long * index, long sub_size, int k, const double * cell_min, const double * cell_max, Buffer<double> & boundary, Buffer<long> & b_indices)
{
 double bound_min[2], bound_max[2];
 bound_min[0] = cell_min[0];
 bound_max[0] = cell_max[0];
 bound_min[1] = cell_min[1];
 bound_max[1] = cell_max[1];
 // Find all atoms belonging to the boundary defined by bound_min and bound_max
 // We put them in the 'boundary' output array
 long bsize = 0;
 for (long i=0;i<sub_size;++i)
 {
  double x = pos[i*3];
  double y = pos[i*3+1];
  double z = pos[i*3+2];
  // 
  int consider = 0;
  switch(k)
  {
   case 0:
        consider = (x > cell_max[0]-cutoff);
        break;
   case 1:
        consider = (x < cell_min[0]+cutoff);
        break;
   case 3:
        consider = (y < cell_min[1]+cutoff);
        break;
   case 2:
        consider = (y > cell_max[1]-cutoff);
        break;
  };
  if (consider)
  {
   boundary.Resize(3*(++bsize));
   boundary.RawBuffer()[3*(bsize-1)] = x;
   boundary.RawBuffer()[3*(bsize-1)+1] = y;
   boundary.RawBuffer()[3*(bsize-1)+2] = z;
   b_indices.Resize(bsize);
   b_indices.RawBuffer()[bsize-1] = index[i];
  }
 }
}

void AtomSet::SwapBoundaries(const CellTopology & top) const
{
 if (top.NumberOfRanks() == 1) return;
 long remote_sizes[4];
 pos_size = LocalSize();
 if (pos_size <= 0) return;
 Buffer<double> boundaries[4], remotes[4];
 Buffer<long> b_indices[4], r_indices[4];
 parallel::Barrier();
 for (int kd=0;kd<2;++kd)
 {
  if (top.RanksAlong(kd) == 1) continue;
  ScanForBoundaries(top, cutoff, pos, index, pos_size, 2*kd, top.cell_min, top.cell_max, boundaries[2*kd], b_indices[2*kd]);
  ScanForBoundaries(top, cutoff, pos, index, pos_size, 2*kd+1, top.cell_min, top.cell_max, boundaries[2*kd+1], b_indices[2*kd+1]);
  for (int kdd=0;kdd<2;++kdd)
  {
   int k = 2*kd+kdd, recv_k = -1, recv_k_prime = -1;
   Buffer<double> tempremote;
   Buffer<long> ind_remote;
   recv_k = top.CopyOneDirectional(k, 700*(k+1), boundaries[k], tempremote);
   remotes[recv_k].Resize(tempremote.Size());
   remotes[recv_k].Copy(tempremote);
   remote_sizes[recv_k] = (tempremote.Size()/3);
   recv_k_prime = top.CopyOneDirectional(k, 800*(k+1), b_indices[k], ind_remote);
   assert (recv_k_prime == recv_k);
   r_indices[recv_k_prime].Resize(ind_remote.Size());
   r_indices[recv_k_prime].Copy(ind_remote);
   assert (remote_sizes[recv_k] == ind_remote.Size());

   // Merge remote boundaries with the local arrays 'pos' and 'index'
   pos = (double *)(realloc(pos, sizeof(double)*3*(pos_size+remote_sizes[recv_k])));
   index = (long *)(realloc(index, sizeof(long)*(pos_size+remote_sizes[recv_k])));

   // Compute the coordinate shift, if needed
   double shift[3] = { 0.0, 0.0, 0.0 };
   GetBoundaryShift(top, recv_k, shift);
   for (long i=pos_size;i<pos_size+remote_sizes[recv_k];++i)
   {
    for (int q=0;q<3;++q) 
        pos[i*3+q] = (remotes[recv_k].RawBuffer())[3*(i-pos_size)+q]+shift[q];
   }
   for (long i=pos_size;i<pos_size+remote_sizes[recv_k];++i)
       index[i] = (r_indices[recv_k].RawBuffer())[i-pos_size];
   pos_size += remote_sizes[recv_k];
  }
 }
 parallel::Barrier();
}

