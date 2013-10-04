/*
 *
 *
 *
 */

#ifndef __ATOMSET_H__
#define __ATOMSET_H__

#include <lpmd/tagcontainer.h>
#include <lpmd/rawatom.h>
#include <lpmd/neighborlist.h>

#ifdef __cplusplus
#include <list>
#include <lpmd/celltopology.h>
#include <lpmd/buffer.h>
#include <lpmd/taskreturnvalue.h>
#include <lpmd/selector.h>
#include <lpmd/topology.h>

namespace lpmd
{
 typedef void (*AtomModifierFunction)(RawAtomSet *, RawCell *);
 typedef void (*AtomReaderFunction)(const RawAtomSet *, const RawCell *);

 //
 //
 class AtomSet: public RawAtomSet
 {
  public:
   AtomSet();
   AtomSet(bool managed);
   AtomSet(const lpmd::CellTopology & topology, bool managed);
   virtual ~AtomSet();
   
   AtomSet & operator=(const AtomSet & sys);

   void Resize(long n);
   long Size() const;
   long Size(int rank) const { return 0; }
   long ExtendedLocalSize() const { return pos_size; }
   long LocalSize() const;

   void SetBoundaryLength(double r);

   double BoundaryLength() const;

   void Clear() { Resize(0); }
   
   void SetTopology(const lpmd::CellTopology & topology) { top = &topology; }

   const lpmd::CellTopology & GetTopology() const { return *top; }

   double TaskTime() const;

   void AddToLog(const char * msg) const;

   void SetNeighborList(const lpmd::NeighborList & nlist) { nl = &nlist; }
   
   const lpmd::NeighborList & GetNeighborList() const 
   { 
    if (nl == 0) throw lpmd::Error("AtomSet: No neighbors list have been set.");
    return (*nl); 
   }
  
   // Methods for executing a Task function
   void RunTask(AtomModifierFunction t, BasicCell & cell, bool useboundaries=false);
   void RunTask(AtomReaderFunction t, const BasicCell & cell, bool useboundaries=false) const;

   // Methods for handling return values
   bool HasReturnValue() const;
   void PutReturnValue(const ReturnValue & r) const;
   const ReturnValue & GetReturnValue();
   void SetReturnType(int vtype) const;
   int GetReturnType() const;

   void ApplyAtomMask(AtomReaderFunction s, const BasicCell & cell, bool inverse=false);
   void RemoveAtomMask();

   // 
   void GetArrays(double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index) const;
   void SetArrays(long n, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index);

   mutable std::list<AtomData> stack;

  private:
    mutable ReturnValue * rvalue_holder;
    mutable int vtype;
    const lpmd::CellTopology * top;
    const lpmd::NeighborList * nl;
    mutable bool pos_used;
    mutable std::list<const char *> log;
    double cutoff;
    void Destroy();
    void Copy(long j, const AtomSet & sys, long i);
    void CorrectLocalAtoms(const CellTopology & top);
    void SendRecvOutsiders(const CellTopology & top, int kd, AtomSet * s_left, AtomSet * s_right);
    void SwapBoundaries(const CellTopology & top) const;
    void SwapCrossingAtoms(const CellTopology & top);
    void ShowLogs() const;
 };

 template <typename T> void PutTaskValue(const AtomSet & aset, const T & value)
 {
  aset.PutReturnValue(*(new TaskReturnValue<T>(value)));
 }

 template <typename T> const T GetTaskValue(AtomSet & aset)
 {
  const TaskReturnValue<T> * trv = &(reinterpret_cast<const TaskReturnValue<T> &>(aset.GetReturnValue()));
  const T tempvalue(trv->GetValue());
  delete trv;
  T globalvalue;
  parallel::SumReduce<T>(const_cast<T *>(&tempvalue), const_cast<T *>(&globalvalue));
  return globalvalue;
 }

} // lpmd
#endif

//
//
//
enum ReturnValues
{
 RET_DOUBLE = 5,
 RET_VECTOR = 10,
 RET_MATRIX = 15,
 RET_NLIST = 20,
 RET_ASELECT = 25
};

#endif

