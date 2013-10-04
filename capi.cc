
#include <cassert>
#include <cstdio>
#include <cstdarg>

#include <lpmd/atomset.h>
#include <lpmd/basiccell.h>
#include <lpmd/orthogonalcell.h>
#include <lpmd/nonorthogonalcell.h>
#include <lpmd/celltopology.h>
#include <lpmd/matrix.h>
#include <lpmd/atomselection.h>

//FIXME:Parece que params hay que eliminarlo ASAP, revisar params.h
#include "params.h"

static inline int fge(double x, double m)
{
 return ((x > m) || (fabs(x-m) < FLOAT_TOL));
}

static inline int fle(double x, double m)
{
 return ((x < m) || (fabs(x-m) < FLOAT_TOL));
}

using namespace lpmd;

#define LOGBUFFERSIZE 10240

//
//
//
extern "C" void ASet_LogMessage(const RawAtomSet * aset, const char * format, ...)
{
 va_list ap;
 va_start(ap, format);
 char * buffer = (char *)malloc(sizeof(char)*LOGBUFFERSIZE);
 vsnprintf(buffer, LOGBUFFERSIZE, format, ap);
 va_end(ap);
 buffer = (char *)realloc(buffer, strlen(buffer)+1);
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 real_aset.AddToLog(buffer);
}

extern "C" void ASet_GetArrays(const RawAtomSet * aset, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 real_aset.GetArrays(pos, vel, acc, aux0, tc, index);
}

static int tokenize(char * str, char ** buffer)
{
 int cw = -1;
 int sl = strlen(str);
 for (int i=0;i<sl;++i)
 {
  if (str[i] == ' ') str[i] = '\0';
 }
 for (int i=0;i<sl;++i)
 {
  if ((str[i] != '\0') && ((i == 0) || (str[i-1] == '\0')))
     buffer[++cw] = (str+i);
 }
 return (cw+1);
}

extern "C" void ASet_GetArraysByName(const RawAtomSet * aset, const char * a_names, ...)
{
 static char buffer[1024];
 char * words[10];
 void * pointers[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
 strncpy(buffer, a_names, 1024);
 int nw = tokenize(buffer, words);
 va_list ap;
 va_start(ap, a_names);
 for (int q=0;q<nw;++q)
 {
  if (!strcmp(words[q], "pos")) pointers[0] = va_arg(ap, void *);
  else if (!strcmp(words[q], "vel")) pointers[1] = va_arg(ap, void *);
  else if (!strcmp(words[q], "acc")) pointers[2] = va_arg(ap, void *);
  else if (!strcmp(words[q], "aux0")) pointers[3] = va_arg(ap, void *);
  else if (!strcmp(words[q], "tag")) pointers[4] = va_arg(ap, void *);
  else if (!strcmp(words[q], "index")) pointers[5] = va_arg(ap, void *);
  else throw InvalidRequest(words[q]);
 }
 va_end(ap); 
 ASet_GetArrays(aset, (double **)(pointers[0]), (double **)(pointers[1]), (double **)(pointers[2]), (double **)(pointers[3]), (Tag **)(pointers[4]), (long **)(pointers[5]));
}

extern "C" void * ASet_GetArray(const RawAtomSet * aset, const char * a_name)
{
 if (!strcmp(a_name, "pos")) { double * pos; ASet_GetArraysByName(aset, "pos", &pos); return pos; }
 else if (!strcmp(a_name, "vel")) { double * vel; ASet_GetArraysByName(aset, "vel", &vel); return vel; }
 else if (!strcmp(a_name, "acc")) { double * acc; ASet_GetArraysByName(aset, "acc", &acc); return acc; }
 else if (!strcmp(a_name, "aux0")) { double * aux0; ASet_GetArraysByName(aset, "aux0", &aux0); return aux0; }
 else if (!strcmp(a_name, "tag")) { Tag * tc; ASet_GetArraysByName(aset, "tag", &tc); return tc; }
 else if (!strcmp(a_name, "index")) { long * index; ASet_GetArraysByName(aset, "index", &index); return index; }
 else throw InvalidRequest(a_name);
 return NULL;
}

extern "C" void ASet_SetArrays(RawAtomSet * aset, long n, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index)
{
 AtomSet & real_aset = static_cast<AtomSet &>(*aset);
 real_aset.SetArrays(n, pos, vel, acc, aux0, tc, index);
}

extern "C" int Cell_AtomBelongsToRank(const RawAtomSet * aset, const RawCell * rrcell, double * pos);

extern "C" void ASet_AddAtom(RawAtomSet * aset, RawCell * cell, const char * a_names, ...)
{
 static char buffer[1024];
 char * words[10];
 strncpy(buffer, a_names, 1024);
 int nw = tokenize(buffer, words);
 va_list ap;
 va_start(ap, a_names);
 double * ipos = NULL, * ivel = NULL, * iacc = NULL, * iaux0 = NULL;
 long iindex = -1;
 Tag itag;
 itag.flags = 0;
 int tag_active = 0;
 for (int q=0;q<nw;++q)
 {
  if (!strcmp(words[q], "pos")) ipos = va_arg(ap, double *);
  else if (!strcmp(words[q], "vel")) ivel = va_arg(ap, double *);
  else if (!strcmp(words[q], "acc")) iacc = va_arg(ap, double *);
  else if (!strcmp(words[q], "aux0")) iaux0 = va_arg(ap, double *);
  else if (!strcmp(words[q], "tag")) { itag.flags = va_arg(ap, long); tag_active = 1; }
  else if (!strcmp(words[q], "index")) iindex = va_arg(ap, long);
  else throw InvalidRequest(words[q]);
 }
 va_end(ap);
 assert (ipos); // At least the position is needed
 static long natoms = 0;
 static double * pos = NULL;
 static double * vel = NULL;
 static double * acc = NULL;
 static double * aux0 = NULL;
 static Tag * tc = NULL;
 static long * index = NULL;
 if (Cell_AtomBelongsToRank(aset, cell, ipos))
 {
  pos = (double *)realloc(pos, sizeof(double)*3*(natoms+1));
  for (int q=0;q<3;++q) pos[natoms*3+q] = ipos[q];
  if (ivel)
  {
   vel = (double *)realloc(vel, sizeof(double)*3*(natoms+1));
   for (int q=0;q<3;++q) vel[natoms*3+q] = ivel[q];
  }
  if (iacc)
  {
   acc = (double *)realloc(acc, sizeof(double)*3*(natoms+1));
   for (int q=0;q<3;++q) acc[natoms*3+q] = iacc[q];
  }
  if (iaux0)
  {
   aux0 = (double *)realloc(aux0, sizeof(double)*3*(natoms+1));
   for (int q=0;q<3;++q) aux0[natoms*3+q] = iaux0[q];
  }
  if (tag_active)
  {
   tc = (Tag *)realloc(tc, sizeof(Tag)*(natoms+1));
   tc[natoms] = itag;
  }
  if (iindex != -1)
  {
   index = (long *)realloc(index, sizeof(long)*(natoms+1));
   index[natoms] = iindex;
  }
  natoms++;
  ASet_SetArrays(aset, natoms, &pos, &vel, &acc, &aux0, &tc, &index);
 }
}

extern "C" long ASet_LocalSize(const RawAtomSet * aset)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 long s = real_aset.LocalSize();
 if (s < 0) return 0;
 else return s;
}

extern "C" long ASet_ExtendedLocalSize(const RawAtomSet * aset)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 long s = real_aset.ExtendedLocalSize();
 if (s < 0) return 0;
 else return s;
}

extern "C" long ASet_TotalSize(const RawAtomSet * aset)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 return real_aset.Size();
}

extern "C" void Matrix_Destroy(RawMatrix * mat); // Prototype

extern "C" void ASet_ReturnValue(const RawAtomSet * aset, int vtype, void * value, long items)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 if (vtype == RET_DOUBLE)
    PutTaskValue<double>(real_aset, *((double *)value));
 else if (vtype == RET_VECTOR)
 {
  double * p = (double *)(value);
  PutTaskValue<Vector>(real_aset, Vector(p[0], p[1], p[2]));
 }
 else if (vtype == RET_NLIST)
 {
  NeighborList nlist((AtomPair *)value, items);
  PutTaskValue<NeighborList>(real_aset, nlist);
 }
 else if (vtype == RET_MATRIX)
 {
  Matrix mat(*((RawMatrix *)(value)));
  PutTaskValue<Matrix>(real_aset, mat);
  Matrix_Destroy((RawMatrix *)(value));
 }
 else if (vtype == RET_ASELECT)
 {
  long * valuecpy = (long *)value;
  AtomSelection asel(valuecpy, items);
  PutTaskValue<AtomSelection>(real_aset, asel);
 }
 else
  EndWithError("Invalid return type in ASet_ReturnValue");
 //
 real_aset.SetReturnType(vtype);
}

extern "C" void ASet_GetNeighborList(const RawAtomSet * aset, RawAtomPair ** nlist, long * nlsize)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 try
 {
  const NeighborList & nl = real_aset.GetNeighborList();
  *nlist = const_cast<RawAtomPair *>(nl.RawBuffer());
  *nlsize = nl.Size();
 }
 catch (Error &) { }
}

extern "C" void ASet_GetCenterOfMass(const RawAtomSet * aset, double * cm)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 double * pos = NULL;
 double lcm[3];
 long n = ASet_LocalSize(aset);
 long total_n = ASet_TotalSize(aset);
 ASet_GetArrays(aset, &pos, NULL, NULL, NULL, NULL, NULL);
 for (int q=0;q<3;++q) lcm[q] = 0.0e0;
 for (long i=0;i<n;++i)
     for (int q=0;q<3;++q) lcm[q] += M*pos[i*3+q];
 PutTaskValue<Vector>(real_aset, Vector(lcm[0], lcm[1], lcm[2]));
 const Vector v = GetTaskValue<Vector>(const_cast<AtomSet &>(real_aset));
 for (int q=0;q<3;++q) cm[q] = (1.0/(M*total_n))*v[q];
}

extern "C" int ASet_Rank(const RawAtomSet * aset)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 const CellTopology & top = real_aset.GetTopology();
 return top.CurrentRank();
}

extern "C" int ASet_NumberOfRanks(const RawAtomSet * aset)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 const CellTopology & top = real_aset.GetTopology();
 return top.NumberOfRanks();
}

extern "C" long ASet_FilterByIndex(const RawAtomSet * aset, long i_from, long i_to, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index)
{
 double * l_pos = NULL, * l_vel = NULL, * l_acc = NULL, * l_aux0 = NULL;
 Tag * l_tc = NULL;
 long * l_index = (long *)ASet_GetArray(aset, "index");
 long n = ASet_LocalSize(aset), cnt = 0;
 for (long i=0;i<n;++i)
 {
  if ((l_index[i] >= i_from) && (l_index[i] <= i_to)) cnt++;
 }
 // FIXME: So much repetition is offensive to the eyes
 if (pos)
 {
  (*pos) = (double *)(malloc(sizeof(double)*3*cnt));
  l_pos = (double *)ASet_GetArray(aset, "pos"); 
 }
 if (vel)
 { 
  (*vel) = (double *)(malloc(sizeof(double)*3*cnt));
  l_vel = (double *)ASet_GetArray(aset, "vel"); 
 }
 if (acc)
 {
  (*acc) = (double *)(malloc(sizeof(double)*3*cnt));
  l_acc = (double *)ASet_GetArray(aset, "acc");
 }
 if (aux0)
 {
  (*aux0) = (double *)(malloc(sizeof(double)*3*cnt));
  l_aux0 = (double *)ASet_GetArray(aset, "aux0");
 }
 if (tc)
 {
  (*tc) = (Tag *)(malloc(sizeof(Tag)*cnt));
  l_tc = (Tag *)ASet_GetArray(aset, "tc");
 }
 if (index) (*index) = (long *)(malloc(sizeof(long)*cnt));
 cnt = 0;
 for (long i=0;i<n;++i)
 {
  if ((l_index[i] >= i_from) && (l_index[i] <= i_to))
  {
   if (pos) { for (int q=0;q<3;++q) (*pos)[3*cnt+q] = l_pos[3*i+q]; }
   if (vel) { for (int q=0;q<3;++q) (*vel)[3*cnt+q] = l_vel[3*i+q]; }
   if (acc) { for (int q=0;q<3;++q) (*acc)[3*cnt+q] = l_acc[3*i+q]; }
   if (aux0) { for (int q=0;q<3;++q) (*aux0)[3*cnt+q] = l_aux0[3*i+q]; }
   if (tc) { (*tc)[cnt] = l_tc[i]; }
   if (index) { (*index)[cnt] = l_index[i]; }
   cnt++;
  }
 }
 return cnt; 
}

template <typename T> T * AddTogether(int rank, int nprocs, int & tsiz, memory::Buffer<char> * chbuf, memory::Buffer<char> & bigbuf)
{
 if (rank == 0)
 {
  tsiz = 0;
  for (int k=0;k<nprocs;++k) tsiz += chbuf[k].Size();
  bigbuf.Resize(tsiz);
  int kpos = chbuf[0].Bytes();
  for (int k=1;k<nprocs;++k) 
  {
   memcpy(bigbuf.RawBuffer()+kpos, chbuf[k].RawBuffer(), chbuf[k].Bytes());
   kpos += chbuf[k].Bytes();
  }
  delete [] chbuf;
  return (T *)(bigbuf.RawBuffer());
 }
 else return NULL;
}

template <typename T> void SwapItem(T & a, T & b)
{
 T tmp = a;
 a = b;
 b = tmp;
}

template <typename T> void SwapArray(T * a, T * b)
{
 T tmp[3];
 for (int qq=0;qq<3;++qq) 
 {
  tmp[qq] = a[qq];
  a[qq] = b[qq];
  b[qq] = tmp[qq];
 }
}

extern "C" void ASet_GetTotalArrays(const RawAtomSet * aset, long i_from, long i_to, int sorted, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index)
{
 double * fpos = NULL, * fvel = NULL, * facc = NULL, * faux0 = NULL;
 Tag * ftc = NULL;
 long * findex = NULL;
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 const CellTopology & top = real_aset.GetTopology();
 long s = ASet_FilterByIndex(aset, i_from, i_to, (pos ? &fpos : NULL), (vel ? &fvel : NULL), (acc ? &facc : NULL), (aux0 ? &faux0 : NULL), (tc ? &ftc : NULL), (index ? &findex : NULL));
 int tsiz = 0;
 // FIXME: So much repetition is offensive to the eyes
 if (pos)
 {
  memory::Buffer<double> smallbuf(fpos, s*3);
  memory::Buffer<char> bigbuf(smallbuf.Bytes(), false);
  memcpy(bigbuf.RawBuffer(), smallbuf.RawBuffer(), smallbuf.Bytes());
  memory::Buffer<char> * chbuf = top.GatherFromAll(0, bigbuf);
  *pos = AddTogether<double>(top.CurrentRank(), top.NumberOfRanks(), tsiz, chbuf, bigbuf);
  tsiz /= (3*sizeof(double));
 }
 if (vel)
 {
  memory::Buffer<double> smallbuf(fvel, s*3);
  memory::Buffer<char> bigbuf(smallbuf.Bytes(), false);
  memcpy(bigbuf.RawBuffer(), smallbuf.RawBuffer(), smallbuf.Bytes());
  memory::Buffer<char> * chbuf = top.GatherFromAll(0, bigbuf);
  *vel = AddTogether<double>(top.CurrentRank(), top.NumberOfRanks(), tsiz, chbuf, bigbuf);
  tsiz /= (3*sizeof(double));
 }
 if (acc)
 {
  memory::Buffer<double> smallbuf(facc, s*3);
  memory::Buffer<char> bigbuf(smallbuf.Bytes(), false);
  memcpy(bigbuf.RawBuffer(), smallbuf.RawBuffer(), smallbuf.Bytes());
  memory::Buffer<char> * chbuf = top.GatherFromAll(0, bigbuf);
  *acc = AddTogether<double>(top.CurrentRank(), top.NumberOfRanks(), tsiz, chbuf, bigbuf);
  tsiz /= (3*sizeof(double));
 }
 if (aux0)
 {
  memory::Buffer<double> smallbuf(faux0, s*3);
  memory::Buffer<char> bigbuf(smallbuf.Bytes(), false);
  memcpy(bigbuf.RawBuffer(), smallbuf.RawBuffer(), smallbuf.Bytes());
  memory::Buffer<char> * chbuf = top.GatherFromAll(0, bigbuf);
  *aux0 = AddTogether<double>(top.CurrentRank(), top.NumberOfRanks(), tsiz, chbuf, bigbuf);
  tsiz /= (3*sizeof(double));
 }
 if (tc)
 {
  memory::Buffer<Tag> smallbuf(ftc, s);
  memory::Buffer<char> bigbuf(smallbuf.Bytes(), false);
  memcpy(bigbuf.RawBuffer(), smallbuf.RawBuffer(), smallbuf.Bytes());
  memory::Buffer<char> * chbuf = top.GatherFromAll(0, bigbuf);
  *tc = AddTogether<Tag>(top.CurrentRank(), top.NumberOfRanks(), tsiz, chbuf, bigbuf);
  tsiz /= sizeof(Tag);
 }
 if (index)
 {
  memory::Buffer<long> smallbuf(findex, s);
  memory::Buffer<char> bigbuf(smallbuf.Bytes(), false);
  memcpy(bigbuf.RawBuffer(), smallbuf.RawBuffer(), smallbuf.Bytes());
  memory::Buffer<char> * chbuf = top.GatherFromAll(0, bigbuf);
  *index = AddTogether<long>(top.CurrentRank(), top.NumberOfRanks(), tsiz, chbuf, bigbuf);
  tsiz /= sizeof(long);
 }
 parallel::Barrier();
 if (sorted && (top.CurrentRank() == 0))
 {
  assert (index);
  // Sort all the available arrays by index
  for (long p=0;p<tsiz-1;++p)
  {
   long min_idx = (*index)[p], kmin = -1;
   for (long k=p;k<tsiz;++k)
   {
    if ((*index)[k] <= min_idx) { min_idx = (*index)[k]; kmin = k; }
   }
   if (pos) SwapArray<double>((*pos+3*p), (*pos+3*kmin));
   if (vel) SwapArray<double>((*vel+3*p), (*vel+3*kmin));
   if (acc) SwapArray<double>((*acc+3*p), (*acc+3*kmin));
   if (aux0) SwapArray<double>((*aux0+3*p), (*aux0+3*kmin));
   if (tc) SwapItem<Tag>((*tc)[p], (*tc)[kmin]);
   SwapItem<long>((*index)[p], (*index)[kmin]);
  }
 }
 parallel::Barrier();
}

extern "C" void ASet_GetTotalArraysByName(const RawAtomSet * aset, long i_from, long i_to, int sorted, const char * a_names, ...)
{
 static char buffer[1024];
 char * words[10];
 void * pointers[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
 long * index = NULL;
 pointers[5] = &index; // Always ask for the index
 int use_index = 0;
 strncpy(buffer, a_names, 1024);
 int nw = tokenize(buffer, words);
 va_list ap;
 va_start(ap, a_names);
 for (int q=0;q<nw;++q)
 {
  if (!strcmp(words[q], "pos")) pointers[0] = va_arg(ap, void *);
  else if (!strcmp(words[q], "vel")) pointers[1] = va_arg(ap, void *);
  else if (!strcmp(words[q], "acc")) pointers[2] = va_arg(ap, void *);
  else if (!strcmp(words[q], "aux0")) pointers[3] = va_arg(ap, void *);
  else if (!strcmp(words[q], "tag")) pointers[4] = va_arg(ap, void *);
  else if (!strcmp(words[q], "index")) { pointers[5] = va_arg(ap, void *); use_index = 1; }
  else throw InvalidRequest(words[q]);
 }
 va_end(ap);
 ASet_GetTotalArrays(aset, i_from, i_to, sorted, (double **)(pointers[0]), (double **)(pointers[1]), (double **)(pointers[2]), (double **)(pointers[3]), (Tag **)(pointers[4]), (long **)(pointers[5]));
 if (!use_index) free(index);
}

extern "C" void * ASet_GetTotalArray(const RawAtomSet * aset, const char * a_name, long i_from, long i_to, int sorted)
{
 if (!strcmp(a_name, "pos")) { double * pos; ASet_GetTotalArraysByName(aset, i_from, i_to, sorted, "pos", &pos); return pos; }
 else if (!strcmp(a_name, "vel")) { double * vel; ASet_GetTotalArraysByName(aset, i_from, i_to, sorted, "vel", &vel); return vel; }
 else if (!strcmp(a_name, "acc")) { double * acc; ASet_GetTotalArraysByName(aset, i_from, i_to, sorted, "acc", &acc); return acc; }
 else if (!strcmp(a_name, "aux0")) { double * aux0; ASet_GetTotalArraysByName(aset, i_from, i_to, sorted, "aux0", &aux0); return aux0; }
 else if (!strcmp(a_name, "tag")) { Tag * tc; ASet_GetTotalArraysByName(aset, i_from, i_to, sorted, "tag", &tc); return tc; }
 else if (!strcmp(a_name, "index")) { long * index; ASet_GetTotalArraysByName(aset, i_from, i_to, sorted, "index", &index); return index; }
 else throw InvalidRequest(a_name);
 return NULL;
}

extern "C" int Cell_AtomBelongsToRank(const RawAtomSet * aset, const RawCell * rrcell, double * pos)
{
 const AtomSet & real_aset = static_cast<const AtomSet &>(*aset);
 const CellTopology & top = real_aset.GetTopology();
 double cell[3];
 cell[0] = rrcell->a[0];  // FIXME: Solo ortogonal!
 cell[1] = rrcell->b[1];  // FIXME: Solo ortogonal!
 cell[2] = rrcell->c[2];  // FIXME: Solo ortogonal!
 int xi = int(floor(top.RanksAlong(0)*(pos[0]*0.999999/cell[0])));
 if (xi < 0) xi += top.RanksAlong(0);
 else if (xi >= top.RanksAlong(0)) xi -= top.RanksAlong(0);
 int yi = int(floor(top.RanksAlong(1)*(pos[1]*0.999999/cell[1])));
 if (yi < 0) yi += top.RanksAlong(1);
 else if (yi >= top.RanksAlong(1)) yi -= top.RanksAlong(1);
 if ((xi == top.PositionAlong(0)) && (yi == top.PositionAlong(1))) return 1;
 else return 0;
}

extern "C" void Cell_GetCartesian(const RawCell * cell, double * fpos, double * pos)
{
 const BasicCell * bcell = static_cast<const BasicCell *>(cell);
 const Vector v = bcell->Cartesian(Vector(fpos));
 for (int q=0;q<3;++q) pos[q] = v[q];
}

extern "C" void Cell_GetFractional(const RawCell * cell, double * pos, double * fpos)
{
 const BasicCell * bcell = static_cast<const BasicCell *>(cell);
 const Vector v = bcell->Fractional(Vector(pos));
 for (int q=0;q<3;++q) fpos[q] = v[q];
}

extern "C" void Cell_GetLocalCell(const RawAtomSet * aset, const RawCell * cell, double * cell_min, double *cell_max)
{
 const AtomSet * real_aset = static_cast<const AtomSet *>(aset);
 const CellTopology * top = &(real_aset->GetTopology());
 for (int q=0;q<3;++q)
 {
  cell_min[q] = top->cell_min[q];
  cell_max[q] = top->cell_max[q];
 }
}

extern "C" void Matrix_Initialize(RawMatrix * mat, int ncols, int nrows)
{
 mat->columns = ncols;
 mat->rows = nrows;
 mat->label = new const char*[ncols]; 
 mat->values = new double*[nrows];
 for (long j=0;j<nrows;++j)
 {
  mat->values[j] = new double[ncols]; 
  for (long i=0;i<ncols;++i) mat->values[j][i] = 0.0e0;
 }
}

extern "C" void Matrix_Destroy(RawMatrix * mat)
{
 delete [] mat->label;
 for (long j=0;j<mat->rows;++j) delete [] mat->values[j];
 delete [] mat->values;
}

