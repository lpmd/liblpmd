/*
 *
 *
 *
 */

#ifndef __LPMD_NEIGHBORLIST_H__
#define __LPMD_NEIGHBORLIST_H__

typedef struct
{
 double dr[3];
 double r2;
 long index_a, index_b;
 long gindex_a, gindex_b;
 short int jlocal;
} RawAtomPair;

#ifdef __cplusplus
#include <lpmd/buffer.h>
namespace lpmd
{
 typedef RawAtomPair AtomPair;
 typedef memory::Buffer<RawAtomPair> NeighborList;
}  // lpmd
#else
typedef struct
{
 RawAtomPair * pair;
 long size;
} NeighborList;
#endif

#endif

