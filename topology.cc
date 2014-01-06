/*
 *
 *
 *
 */


#ifndef SERIAL
#include <mpi.h>
#endif


#include <vector>
#include <lpmd/vector.h>
#include <lpmd/buffer.h>
#include <lpmd/topology.h>
#include <lpmd/neighborlist.h>
#include <lpmd/atomselection.h>
#include <lpmd/matrix.h>
#include <lpmd/util.h>
#include <lpmd/capi.h>
#include "mtwist.h"

using namespace parallel;
using namespace memory;

int iv[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };

//
//
int parallel::InitializeCommunication(int * argc, char *** argv) 
{ 
 int nprocs = 1;
#ifndef SERIAL
 MPI_Init(argc, argv); 
 MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
#endif

 // A falta de un mejor lugar
 InitTagRegistry();
 return nprocs;
}

void parallel::FinishCommunication() 
{ 
 parallel::Barrier(); 
#ifndef SERIAL
 MPI_Finalize(); 
#endif
}

void parallel::Barrier() 
{ 
#ifndef SERIAL
 MPI_Barrier(MPI_COMM_WORLD); 
#endif
}

void parallel::SumReduce(double * send, double * recv)
{
#ifndef SERIAL
 MPI_Allreduce(send, recv, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
#else
 *recv = *send;
#endif
}

void parallel::SumReduce(long * send, long * recv)
{
#ifndef SERIAL
 MPI_Allreduce(send, recv, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
#else
 *recv = *send;
#endif
}

void parallel::SumReduce(int * send, int * recv)
{
#ifndef SERIAL
 MPI_Allreduce(send, recv, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD);
#else
 *recv = *send;
#endif
}

//
namespace parallel
{
 template<> void SumReduce<double>(double * send, double * recv) { SumReduce(send, recv); }
 template<> void SumReduce<lpmd::Vector>(lpmd::Vector * send, lpmd::Vector * recv)
 {
#ifndef SERIAL
  double asend[3], arecv[3];
  for (int q=0;q<3;++q) asend[q] = (*send)[q];
  MPI_Allreduce(asend, arecv, 3, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD); 
  for (int q=0;q<3;++q) (*recv)[q] = arecv[q];
#else
  *recv = *send;
#endif
 }

 template<> void SumReduce<std::vector<long> >(std::vector<long> * send, std::vector<long> * recv) 
 { 
  *recv = *send;
 }

 template<> void SumReduce<lpmd::NeighborList>(lpmd::NeighborList * send, lpmd::NeighborList * recv) 
 { 
  *recv = *send;
 }

 template<> void SumReduce<lpmd::Matrix>(lpmd::Matrix * send, lpmd::Matrix * recv)
 { 
  *recv = *send;
 }
 
 template<> void SumReduce<lpmd::AtomSelection>(lpmd::AtomSelection * send, lpmd::AtomSelection * recv) 
 { 
  *recv = *send;
 }
}

//
//
Topology::Topology(int nx, int ny)
{
 int k1[2];
 mt_seed();

#ifndef SERIAL
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
#else
 rank = 0;
 nprocs = 1;
 if ((nx != 1) || (ny != 1))
    lpmd::EndWithError("This liblpmd was compiled without MPI support.");
#endif
 assert(nprocs == nx*ny);

 vproc[0] = nx;
 vproc[1] = ny;

 vid[0] = rank / vproc[1];
 vid[1] = rank % vproc[1];

 for (int ku=0;ku<4;ku++)
 {
  for (int a=0;a<2;a++) k1[a] = (vid[a]+iv[ku][a]+vproc[a]) % vproc[a];
  nn[ku] = k1[0]*vproc[1]+k1[1];
 }

 for (int a=0;a<2;a++)
 {
  if (vproc[a] == 1) parity[a] = 2;
  else if (vid[a] % 2 == 0) parity[a] = 0;
  else parity[a] = 1;
 }
}

Topology::~Topology() { }

void Topology::CollectDirectionalSizes(int kd, int kdd, Buffer<double> locals[4], int rsizes[4])
{
 int ku = 2*kd+kdd, anti_ku = 2*kd+1-kdd;
#ifndef SERIAL
 int this_rsize = -1;
 int inode = nn[ku];
 MPI_Status status;
 if (parity[kd] == 0)
 {
  long s = locals[ku].Size();
  MPI_Send(&s, 1, MPI_LONG, inode, 5+anti_ku, MPI_COMM_WORLD);
  MPI_Recv(&this_rsize, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  rsizes[status.MPI_TAG-5] = this_rsize;
 }
 else if (parity[kd] == 1)
 {
  long s = locals[ku].Size();
  MPI_Recv(&this_rsize, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  rsizes[status.MPI_TAG-5] = this_rsize;
  MPI_Send(&s, 1, MPI_LONG, inode, 5+anti_ku, MPI_COMM_WORLD);
 }
 else { rsizes[anti_ku] = locals[ku].Size(); } // 1 proc
 Barrier();
#else
 rsizes[anti_ku] = locals[ku].Size();
#endif
}

int Topology::CopyOneDirectional(int k, int tag, Buffer<double> & localbuf, Buffer<double> & remotebuf) const
{
 int recv_k = -1;
#ifndef SERIAL 
 MPI_Status status;
 MPI_Request request;
 int kd = k / 2;
 int kdd = k % 2;
 int max_rsize = -1;
 int anti_ku = 2*kd+1-kdd;
 int inode = nn[anti_ku];
 if (parity[kd] == 0)
 {
  MPI_Isend(localbuf.RawBuffer(), localbuf.Size(), MPI_DOUBLE, inode, 10+tag+anti_ku, MPI_COMM_WORLD, &request);
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Get_count(&status, MPI_DOUBLE, &max_rsize);
  remotebuf.Resize(max_rsize);
  MPI_Recv(remotebuf.RawBuffer(), max_rsize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  recv_k = status.MPI_TAG-10-tag;
  MPI_Wait(&request, MPI_STATUS_IGNORE);
 }
 else if (parity[kd] == 1)
 {
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Get_count(&status, MPI_DOUBLE, &max_rsize);
  remotebuf.Resize(max_rsize);
  MPI_Recv(remotebuf.RawBuffer(), max_rsize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  recv_k = status.MPI_TAG-10-tag;
  MPI_Isend(localbuf.RawBuffer(), localbuf.Size(), MPI_DOUBLE, inode, 10+tag+anti_ku, MPI_COMM_WORLD, &request);
  MPI_Wait(&request, MPI_STATUS_IGNORE);
 }
 else 
 { 
  if (localbuf.Size() > 0) remotebuf.Copy(localbuf);
  recv_k = k;
 } // 1 proc
 Barrier();
#else
 if (localbuf.Size() > 0) remotebuf.Copy(localbuf);
 recv_k = k;
#endif
 return recv_k;
}

//
//
//
int Topology::CopyOneDirectional(int k, int tag, Buffer<long> & localbuf, Buffer<long> & remotebuf) const
{
 int recv_k = -1;
#ifndef SERIAL 
 MPI_Status status;
 MPI_Request request;
 int kd = k / 2;
 int kdd = k % 2;
 int max_rsize = -1;
 int anti_ku = 2*kd+1-kdd;
 int inode = nn[anti_ku];
 if (parity[kd] == 0)
 {
  MPI_Isend(localbuf.RawBuffer(), localbuf.Size(), MPI_LONG, inode, 10+tag+anti_ku, MPI_COMM_WORLD, &request);
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Get_count(&status, MPI_LONG, &max_rsize);
  remotebuf.Resize(max_rsize);
  MPI_Recv(remotebuf.RawBuffer(), max_rsize, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  recv_k = status.MPI_TAG-10-tag;
  MPI_Wait(&request, MPI_STATUS_IGNORE);
 }
 else if (parity[kd] == 1)
 {
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Get_count(&status, MPI_LONG, &max_rsize);
  remotebuf.Resize(max_rsize);
  MPI_Recv(remotebuf.RawBuffer(), max_rsize, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  recv_k = status.MPI_TAG-10-tag;
  MPI_Isend(localbuf.RawBuffer(), localbuf.Size(), MPI_LONG, inode, 10+tag+anti_ku, MPI_COMM_WORLD, &request);
  MPI_Wait(&request, MPI_STATUS_IGNORE);
 }
 else 
 { 
  if (localbuf.Size() > 0) remotebuf.Copy(localbuf);
  recv_k = k;
 } // 1 proc
 Barrier();
#else
 if (localbuf.Size() > 0) remotebuf.Copy(localbuf);
 recv_k = k;
#endif
 return recv_k;
}

//
//
//
void Topology::CopyDirectional(Buffer<double> locals[4], Buffer<double> remotes[4])
{
#ifndef SERIAL
 int max_rsize = -1, rsizes[4];
 Barrier();
 // 
 for (int kd=0;kd<2;++kd)
  for (int kdd=0;kdd<2;++kdd) CollectDirectionalSizes(kd, kdd, locals, rsizes);
 //
  for (int q=0;q<4;++q) 
      if (rsizes[q] > max_rsize) max_rsize = rsizes[q];
 Barrier();
 Buffer<double> buffer(max_rsize);
 //
 for (int kd=0;kd<2;++kd)
  for (int kdd=0;kdd<2;++kdd)
  {
   int ku=2*kd+kdd, anti_ku=2*kd+1-kdd;
   CopyOneDirectional(ku, 10+anti_ku, locals[ku], remotes[anti_ku]);
  }
#else
 for (int kd=0;kd<2;++kd)
  for (int kdd=0;kdd<2;++kdd)
  {
   int ku=2*kd+kdd, anti_ku=2*kd+1-kdd;
   remotes[anti_ku].Resize(locals[ku].Size());
   remotes[anti_ku].Copy(locals[ku]);
  }
#endif
}

Buffer<char> * Topology::GatherFromAll(int rank0, Buffer<char> & buf) const
{
#ifndef SERIAL
 Barrier();
 if (rank == rank0)
 {
  MPI_Status status;
  Buffer<char> * chbuf = new Buffer<char>[nprocs];
  for (int k=0;k<nprocs;++k)
  {
   int size;
   if (k == rank0) 
   {
    chbuf[k].Resize(buf.Size());
    chbuf[k].Copy(buf);
    continue;
   }
   MPI_Probe(k, 90, MPI_COMM_WORLD, &status);
   MPI_Get_count(&status, MPI_CHAR, &size);
   chbuf[k].Resize(size);
   MPI_Recv(chbuf[k].RawBuffer(), size, MPI_CHAR, k, 90, MPI_COMM_WORLD, &status);
  }
  Barrier();
  return chbuf;
 }
 else 
 {
  MPI_Send(buf.RawBuffer(), buf.Size(), MPI_CHAR, rank0, 90, MPI_COMM_WORLD);
  Barrier();
  return NULL;
 }
#else
 Buffer<char> * chbuf = new Buffer<char>[nprocs];
 chbuf[0].Resize(buf.Size());
 chbuf[0].Copy(buf);
 return chbuf;
#endif
}

