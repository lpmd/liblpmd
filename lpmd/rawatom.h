/*
 *
 *
 *
 */

#ifndef __RAWATOM_H__
#define __RAWATOM_H__

#include <lpmd/tagcontainer.h>

#ifdef __cplusplus

//
class AtomData
{
 public:
   AtomData(long _n, double * _pos, double * _vel, double * _acc, double * _aux0, Tag * _tc, long * _index)
   {
    sub_size = _n;
    pos = _pos;
    vel = _vel;
    acc = _acc;
    aux0 = _aux0;
    tc = _tc;
    index = _index;
   }
   AtomData(): sub_size(0), pos(0), vel(0), acc(0), aux0(0), tc(0) { }

   long sub_size;
   double * pos, * vel, * acc, * aux0;
   Tag * tc;
   long * index;
};

#else
#define mutable 
#endif

//
typedef struct
{
 long size;
 mutable long total_size;
 mutable long pos_size;
 mutable long sub_size;
 long last_resize;
 mutable double * pos;
 double * vel, * acc, * aux0;
 Tag * tc;
 mutable long * index;
 int _managed;
} RawAtomSet;

#endif

