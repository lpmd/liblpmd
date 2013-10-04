/*
 *
 *
 *
 */

#ifndef __PARAMD_TOPOLOGY_H__
#define __PARAMD_TOPOLOGY_H__

#include <lpmd/buffer.h>

namespace parallel
{

 //
 //
 int InitializeCommunication(int * argc, char *** argv);
 
 void FinishCommunication();
 
 void Barrier();
 
 void SumReduce(int * send, int * recv);

 void SumReduce(long * send, long * recv);

 void SumReduce(double * send, double * recv);

 template <typename T> void SumReduce(T * send, T * recv);

 //
 //
 class Topology
 {
  public:
     Topology(int nx, int ny);
     virtual ~Topology();

     int CopyOneDirectional(int k, int tag, memory::Buffer<double> & localbuf, memory::Buffer<double> & remotebuf) const;
     
     int CopyOneDirectional(int k, int tag, memory::Buffer<long> & localbuf, memory::Buffer<long> & remotebuf) const;

     void CopyDirectional(memory::Buffer<double> locals[4], memory::Buffer<double> remotes[4]);

     memory::Buffer<char> * GatherFromAll(int rank0, memory::Buffer<char> & buf) const;

     int CurrentRank() const { return rank; }

     int NumberOfRanks() const { return nprocs; }

     int PositionAlong(int q) const { return (q < 2 ? vid[q] : 0); }

     int RanksAlong(int q) const { return (q < 2 ? vproc[q] : 1); }

  private:
     int rank, nprocs, nn[4];
     int parity[2], vid[2], vproc[2];

     void CollectDirectionalSizes(int kd, int kdd, memory::Buffer<double> locals[4], int rsizes[4]);
 };

} // parallel

#endif

