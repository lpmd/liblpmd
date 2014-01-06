/*
 *
 *
 *
 */

#ifndef __LPMD_CAPI_H__
#define __LPMD_CAPI_H__

#include <lpmd/atomset.h>
#include <lpmd/basiccell.h>
#include <lpmd/tagcontainer.h>
#include <lpmd/neighborlist.h>
#include <lpmd/matrix.h>

typedef RawAtomSet AtomSet;
typedef RawCell Cell;
typedef RawAtomPair AtomPair;
typedef RawMatrix Matrix;

extern char * TagRegistry[64]; // This is a hardcoded limit

extern void ASet_LogMessage(const RawAtomSet * aset, const char * format, ...);

extern void ASet_GetArrays(const RawAtomSet * aset, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index);

extern void ASet_GetArraysByName(const RawAtomSet * aset, const char * a_names, ...);

extern void ASet_AddAtom(RawAtomSet * aset, RawCell * cell, const char * a_names, ...);

extern void * ASet_GetArray(const RawAtomSet * aset, const char * a_name);

extern void ASet_SetArrays(RawAtomSet * aset, long n, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index);

extern long ASet_LocalSize(const RawAtomSet * aset);

extern long ASet_ExtendedLocalSize(const RawAtomSet * aset);

extern long ASet_TotalSize(const RawAtomSet * aset);

extern void * ASet_GetTotalArray(const RawAtomSet * aset, const char * a_name, long i_from, long i_to, int sorted);

extern void ASet_ReturnValue(const RawAtomSet * aset, int vtype, void * value, long items);

extern void ASet_GetNeighborList(const RawAtomSet * aset, RawAtomPair ** nlist, long * nlsize);

extern void ASet_GetCenterOfMass(const RawAtomSet * aset, double * cm);

extern int ASet_Rank(const RawAtomSet * aset);

extern int ASet_NumberOfRanks(const RawAtomSet * aset);

extern int Cell_AtomBelongsToRank(const AtomSet * aset, const Cell * cell, double * pos);

inline double Cell_OrthoDistance(const RawCell * cell, double * dr, double * pos_i, double * pos_j)
{
 double m, mhalf, *dd, r2 = 0.0e0;
 double modv[3] = { cell->a[0], cell->b[1], cell->c[2] };
 for (int q=0;q<3;++q)
 {
  dr[q] = pos_j[q]-pos_i[q];
  //if (!p[i]) continue;
  m = modv[q];
  mhalf = 0.5*m;
  dd = dr+q;
  if (*dd >= mhalf) (*dd) -= m;
  else if (*dd < -mhalf) (*dd) += m;
  r2 += (dr[q]*dr[q]);
 }
 return r2;
}

extern void Cell_GetLocalCell(const AtomSet * aset, const Cell * cell, double * cell_min, double *cell_max);

extern void Cell_GetCartesian(const RawCell * cell, double * fpos, double * pos);

extern void Cell_GetFractional(const RawCell * cell, double * pos, double * fpos);

extern void Matrix_Initialize(Matrix * mat, int ncols, int nrows);

extern void Matrix_Destroy(Matrix * mat);

inline int HasTag(const Tag tag, int key) { return (tag.flags & (1 << key)); }

inline void SetTag(Tag * tag, int key) { tag->flags |= (1 << key); }

inline void UnsetTag(Tag * tag, int key) { tag->flags &= ~(1 << key); }

void InitTagRegistry();

inline int RegisterTag(const char * name) 
{   
 printf("DEBUG RegisterTag %s\n", name);
 for (int i=0;i<64;i++) 
     if (TagRegistry[i] == NULL) { TagRegistry[i] = (char *)(name); return i; }
 return -1;
}

inline int LookupTag(const char * name) 
{ 
 printf("DEBUG LookupTag %s\n", name);
 for (int i=0;i<64;i++)
 {
  if ((TagRegistry[i] != NULL) && (!strcmp(TagRegistry[i], name))) return i;
 }
 return -1; 
} 

#endif

