/*
 *
 *
 *
 */

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include <lpmd/atomset.h>
#include <lpmd/buffer.h>

using namespace std;
using namespace memory;
using namespace lpmd;

double send_buffer[2][20480];
long index_send_buffer[2][20480];

void AtomSet::CorrectLocalAtoms(const lpmd::CellTopology & top)
{
 for (long i=0;i<size;++i)
  for (int q=0;q<3;++q)
  {
   const double ll = top.GetCellDimensions()[q];
   if (pos[i*3+q] < 0.0) pos[i*3+q] -= ll*floor(pos[i*3+q]/ll);
   else if (pos[i*3+q] >= ll) pos[i*3+q] -= ll*floor(pos[i*3+q]/ll);
   //assert ((pos[i*3+q] >= 0.0) && (pos[i*3+q] < ll));
  }
}

void AtomSet::SendRecvOutsiders(const lpmd::CellTopology & top, int kd, lpmd::AtomSet * s_left, lpmd::AtomSet * s_right)
{
 // Send atoms in b_left and b_right and receive the neighbors' buffers
 parallel::Barrier();
 
 lpmd::AtomSet * sys_array[2] = { s_right, s_left };
 long b_length[2];
 b_length[0] = sys_array[0]->size;
 b_length[1] = sys_array[1]->size;

 // Need another pair of send buffers to send the serialized 
 // tag data...

 // Fills the send buffers
 for (int z=0;z<2;++z)
 {
  for (long i=0;i<b_length[z];++i)
  {
   for (int q=0;q<3;++q) 
   {
    send_buffer[z][i*3+q] = sys_array[z]->pos[i*3+q];
    send_buffer[z][(b_length[z]+i)*3+q] = sys_array[z]->vel[i*3+q];
    send_buffer[z][(2*b_length[z]+i)*3+q] = sys_array[z]->acc[i*3+q];
    send_buffer[z][(3*b_length[z]+i)*3+q] = sys_array[z]->aux0[i*3+q];
   }
  }
 }
 // Fills in the 'index' buffer
 for (int z=0;z<2;++z)
  for (long i=0;i<b_length[z];++i)
      index_send_buffer[z][i] = sys_array[z]->index[i];
 for (int kdd=0;kdd<2;++kdd)
 {
  int send_k = 2*kd+kdd, recv_k = -1, recv_k_prime = -1;
  long rsize = -1, rsize_prime = -1;
  parallel::Barrier();

  // Wraps the memory in a Buffer and sends it  // receives it
  memory::Buffer<double> templocal(send_buffer[kdd], 3*4*b_length[kdd]);
  memory::Buffer<double> tempremote;
  recv_k = top.CopyOneDirectional(send_k, 38700*(send_k+1), templocal, tempremote);
  rsize = tempremote.Size();
  //
  memory::Buffer<long> itremote;
  memory::Buffer<long> itlocal(index_send_buffer[kdd], b_length[kdd]);
  recv_k_prime = top.CopyOneDirectional(send_k, 38800*(send_k+1), itlocal, itremote);
  rsize_prime = itremote.Size();
  assert (recv_k_prime == recv_k);
  assert (rsize_prime == rsize/(3*4));

  parallel::Barrier();
  rsize /= (3*4);

  // 
  // Send / receive the serialized tag data for the crossing atoms 
  // FIXME: NOT implemented yet
  //

  // Add the received buffers to the new local atoms 
  Resize(size+rsize);
  for (long m=0;m<rsize;++m)
  {
   for (int q=0;q<3;++q) 
   {
    pos[(size+m)*3+q] = tempremote.RawBuffer()[m*3+q];
    vel[(size+m)*3+q] = tempremote.RawBuffer()[(rsize+m)*3+q];
    acc[(size+m)*3+q] = tempremote.RawBuffer()[(2*rsize+m)*3+q];
    aux0[(size+m)*3+q] = tempremote.RawBuffer()[(3*rsize+m)*3+q];
   }
  }
  for (long m=0;m<rsize;++m) index[size+m] = (itremote.RawBuffer())[m];
  parallel::Barrier();
  size += rsize;
 }
 sub_size = size;
}

void AtomSet::SwapCrossingAtoms(const lpmd::CellTopology & top)
{
 if (top.NumberOfRanks() == 1) return;
 long total_size = Size();
 // Save the boundary atoms in a separate array
 long bsize = (pos_size-sub_size);
 if (bsize <= 0) return;
 Buffer<double> boundary(3*bsize);
 memcpy(boundary.RawBuffer(), pos+3*sub_size, boundary.Bytes());
 parallel::Barrier();

 for (int kd=0;kd<2;++kd)
 {
  lpmd::AtomSet s_left, s_right;
  if (top.RanksAlong(kd) == 1) continue;
  s_left.Resize(sub_size);
  s_right.Resize(sub_size);

  lpmd::AtomSet newsys(true);
  newsys.Resize(sub_size);

  s_left.size = s_right.size = size = 0;

  // Collect atoms outside the box in the kd direction
  for (long i=0;i<sub_size;++i)
  {
   if (pos[i*3+kd] <= top.cell_min[kd]) s_left.Copy((s_left.size)++, *this, i);
   else if (pos[i*3+kd] > top.cell_max[kd]) s_right.Copy((s_right.size)++, *this, i);
   else newsys.Copy(size++, *this, i);
  }

  //newsys.Resize(size); // This resize makes a weird segfault in big systems...
  (*this) = newsys;

  SendRecvOutsiders(top, kd, &s_left, &s_right);
 // sub_size = size;
 }
 
 // Add the boundary atoms to the new local atoms
 pos = (double *)(realloc(pos, sizeof(double)*3*(size+bsize)));
 memcpy(pos+3*size, boundary.RawBuffer(), boundary.Bytes());
 pos_size = sub_size+bsize;
 if (total_size > 0) assert(total_size = Size());
 CorrectLocalAtoms(top);
}

