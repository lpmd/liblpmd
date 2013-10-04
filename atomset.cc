/*
 *
 *
 *
 */

#include <iostream>
#include <cassert>
#include <vector>
#include <unistd.h>

#include <lpmd/atomset.h>

// 
#include <lpmd/orthogonalcell.h>
#include <lpmd/nonorthogonalcell.h>
#include <lpmd/neighborlist.h>
#include <lpmd/atomselection.h>
#include <lpmd/matrix.h>

#include <lpmd/buffer.h>
#include <lpmd/tagcontainer.h>
#include <lpmd/util.h>
#include <lpmd/timer.h>

static double task_time = 0.0e0;

const Tag NULLTAG = { 0 };

//
namespace lpmd
{

AtomSet::AtomSet(): rvalue_holder(0), vtype(-1), top(0), nl(0)
{ 
 pos = vel = acc = aux0 = 0;
 tc = 0;
 index = 0;
 size = last_resize = 0;
 sub_size = -1;
 _managed = true;
}

AtomSet::AtomSet(bool managed): rvalue_holder(0), vtype(-1), top(0), nl(0)
{
 pos = vel = acc = aux0 = 0;
 tc = 0;
 index = 0;
 size = last_resize = 0;
 sub_size = -1;
 _managed = managed;
}

AtomSet::AtomSet(const lpmd::CellTopology & topology, bool managed): rvalue_holder(0), vtype(-1), top(0), nl(0)
{
 pos = vel = acc = aux0 = 0;
 tc = 0;
 index = 0;
 size = last_resize = 0;
 sub_size = -1;
 _managed = managed;
 SetTopology(topology);
}

void AtomSet::Destroy()
{
 if (_managed)
 {
  free(pos);
  free(vel);
  free(acc);
  free(aux0);
  free(index);
  free(tc);
 }
}

AtomSet::~AtomSet() 
{ 
 Destroy(); 
 for (std::list<const char *>::const_iterator it=log.begin();it!=log.end();++it)
 {
  free ((void *)(*it));
 }
}

void AtomSet::SetBoundaryLength(double r)
{
 const lpmd::CellTopology & top = GetTopology();
 const double * cell = top.GetCellDimensions();
 for (int q=0;q<2;++q)
     if (cell[q]/top.RanksAlong(q) < r) throw CutoffTooLarge();
 cutoff = r;
}

double AtomSet::BoundaryLength() const { return cutoff; }

long AtomSet::Size() const
{
 parallel::Barrier();
 total_size = -1;
 parallel::SumReduce(&sub_size, &total_size);
 return total_size;
}

double AtomSet::TaskTime() const { return task_time; } 

void AtomSet::Resize(long n)
{
 if (n == last_resize) return;
 if (n == 0)
 {
  pos = vel = acc = aux0 = 0;
  tc = 0;
  index = 0;
 }
 else
 {
  pos = (double *)(realloc(pos, sizeof(double)*3*n));
  vel = (double *)(realloc(vel, sizeof(double)*3*n));
  acc = (double *)(realloc(acc, sizeof(double)*3*n));
  aux0 = (double *)(realloc(aux0, sizeof(double)*3*n));
  tc = (Tag *)(realloc(tc, sizeof(Tag)*n));
  index = (long *)(realloc(index, sizeof(long)*n));
 }
 last_resize = n;
}

void AtomSet::Copy(long j, const AtomSet & sys, long i)
{
 for (int q=0;q<3;++q) 
 {
  pos[3*j+q] = sys.pos[i*3+q];
  vel[3*j+q] = sys.vel[i*3+q];
  acc[3*j+q] = sys.acc[i*3+q];
  aux0[3*j+q] = sys.aux0[i*3+q];
 }
 tc[j] = sys.tc[i];
 if (index != NULL) index[j] = sys.index[i];
}

AtomSet & AtomSet::operator=(const AtomSet & sys)
{
 long n = sub_size;
 assert (sub_size >= sys.sub_size);
 Resize(n);
 if (pos != NULL) memcpy(pos, sys.pos, sizeof(double)*3*n);
 if (vel != NULL) memcpy(vel, sys.vel, sizeof(double)*3*n);
 if (acc != NULL) memcpy(acc, sys.acc, sizeof(double)*3*n);
 if (aux0 != NULL) memcpy(aux0, sys.aux0, sizeof(double)*3*n);
 if (tc != NULL) memcpy(tc, sys.tc, sizeof(Tag)*n);
 if (index != NULL) memcpy(index, sys.index, sizeof(long)*n);
 sub_size = n;
 return (*this);
}

long AtomSet::LocalSize() const
{
 return ((stack.size() == 0) ? sub_size : stack.front().sub_size);
}

void AtomSet::RunTask(AtomModifierFunction t, BasicCell & cell, bool useboundaries)
{ 
 SetReturnType(-1);
 double * cdim = (double *)(top->GetCellDimensions());
 for (int q=0;q<3;++q) cdim[q] = cell[q].Module();
 if ((useboundaries) && (&top != 0)) SwapBoundaries(*top);
 // Executes the task function
 stack.push_front(AtomData(LocalSize(), pos, vel, acc, aux0, tc, index));
 //
 for (int q=0;q<3;++q)
 {
  cell.a[q] = cell[0][q];
  cell.b[q] = cell[1][q];
  cell.c[q] = cell[2][q];
 }
 //
 lpmd::Timer t_task;
 pos_used = false;
 t_task.Start();
 (*t)(this, &cell);
 t_task.Stop();
 task_time += t_task.Elapsed();
 // Feedbacks the changes in cell 
 const lpmd::Vector a(cell.a[0], cell.a[1], cell.a[2]);
 const lpmd::Vector b(cell.b[0], cell.b[1], cell.b[2]);
 const lpmd::Vector c(cell.c[0], cell.c[1], cell.c[2]);
 lpmd::NonOrthogonalCell nocell(a, b, c);
 if (nocell.IsOrthogonal()) { cell = lpmd::OrthogonalCell(cell.a[0], cell.b[1], cell.c[2]); }
 else { cell = nocell; }
 //
 stack.pop_front();
 //
#ifdef SMART_SWAPCROSSING
 if ((&top != 0) && (pos_used))
 {
  int flag = 0, reducedflag = 0;
  for (int q=0;q<2;++q)
  {
   for (long i=0;i<LocalSize();++i)
   {
    if ((pos[i*3+q] <= top->cell_min[q]) || (pos[i*3+q] > top->cell_max[q])) 
    {
     flag = 1;
     break;
    }
   }
   if (flag) break;
  }
  parallel::SumReduce(&flag, &reducedflag);
  if (reducedflag) SwapCrossingAtoms(*top);
 }
#else
 if ((&top != 0) && (pos_used)) SwapCrossingAtoms(*top);
#endif
 ShowLogs();
}

void AtomSet::RunTask(AtomReaderFunction t, const BasicCell & cell, bool useboundaries) const
{
 SetReturnType(-1);
 double * cdim = (double *)(top->GetCellDimensions());
 for (int q=0;q<3;++q) cdim[q] = cell[q].Module();
 if ((useboundaries) && (&top != 0)) SwapBoundaries(*top);
 // Executes the task function
 stack.push_front(AtomData(LocalSize(), pos, vel, acc, aux0, tc, index));
 //
 lpmd::NonOrthogonalCell newcell(cell[0], cell[1], cell[2]);
 for (int q=0;q<3;++q)
 {
  newcell.a[q] = cell[0][q];
  newcell.b[q] = cell[1][q];
  newcell.c[q] = cell[2][q];
 }
 //
 lpmd::Timer t_task;
 t_task.Start();
 (*t)(this, &newcell);
 t_task.Stop();
 task_time += t_task.Elapsed();
 stack.pop_front();
 ShowLogs();
}

void AtomSet::ShowLogs() const
{
 const lpmd::CellTopology & top = GetTopology();
 int ranks = top.NumberOfRanks();
 for (int k=0;k<ranks;++k)
 {
  parallel::Barrier();
  if (top.CurrentRank() == k)
  {
   for (std::list<const char *>::const_iterator it=log.begin();it!=log.end();++it)
   {
    std::cerr << k << ": " << (*it) << '\n';
    usleep(100);
    free ((void *)(*it));
   }
   log.clear();
  }
  parallel::Barrier();
 }
}

void AtomSet::ApplyAtomMask(AtomReaderFunction s, const BasicCell & cell, bool inverse)
{
 AtomData filtered;
 filtered.index = NULL;
 // The selector is applied
 RunTask(s, cell);
 lpmd::AtomSelection selected = GetTaskValue<lpmd::AtomSelection>(*this);
 // 
 double * rpos = NULL, * rvel = NULL, * racc = NULL, * raux0 = NULL;
 Tag * rtc = NULL;
 long * rindex = NULL;
 filtered.sub_size = selected.Size();
 filtered.pos = (double *)(malloc(sizeof(double)*(3*selected.Size())));
 filtered.vel = (double *)(malloc(sizeof(double)*(3*selected.Size())));
 filtered.acc = (double *)(malloc(sizeof(double)*(3*selected.Size())));
 filtered.aux0 = (double *)(malloc(sizeof(double)*(3*selected.Size())));
 filtered.tc = (Tag *)(malloc(sizeof(Tag)*selected.Size()));
 filtered.index = (long *)(malloc(sizeof(long)*(selected.Size())));
 //
 AtomData data;
 if (stack.size() == 0) data = AtomData(sub_size, pos, vel, acc, aux0, tc, index);
 else data = stack.front();
 rpos = data.pos;
 rvel = data.vel;
 racc = data.acc;
 raux0 = data.aux0;
 rtc = data.tc;
 rindex = data.index;
 //
 if (inverse)
 {
  // Invert the selection "in place"
  char * buff = (char *)(malloc(Size()));
  for (long i=0;i<Size();++i) buff[i] = 1;
  for (long i=0;i<selected.Size();++i) buff[selected[i]] = 0;
  selected.Resize(Size()-selected.Size());
  unsigned long k = 0;
  for (long i=0;i<Size();++i)
      if (buff[i]) selected[k++] = i;
  free(buff);
 }
 for (long i=0;i<selected.Size();++i)
 {
  unsigned long k = selected[i];
  for (int q=0;q<3;++q)
  {
   filtered.pos[i*3+q] = rpos[k*3+q];
   filtered.vel[i*3+q] = rvel[k*3+q];
   filtered.acc[i*3+q] = racc[k*3+q];
   filtered.aux0[i*3+q] = raux0[k*3+q];
  }
  filtered.tc[i] = rtc[k];
  filtered.index[i] = rindex[k];
 }
 stack.push_front(filtered);
}

void AtomSet::RemoveAtomMask() 
{ 
 AtomData filtered = stack.front();
 free(filtered.pos);
 free(filtered.vel);
 free(filtered.acc);
 free(filtered.aux0);
 free(filtered.tc);
 free(filtered.index);
 stack.pop_front(); 
}

void AtomSet::GetArrays(double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index) const
{
 if (stack.size() == 0)
 {
  std::cerr << "[Error] Using GetArrays() outside a Task is forbidden.\n";
  exit(1);
 }
 AtomData data = stack.front();
 if (pos != NULL) { *pos = data.pos; pos_used = true; }
 if (vel != NULL) *vel = data.vel;
 if (acc != NULL) *acc = data.acc;
 if (aux0 != NULL) *aux0 = data.aux0;
 if (tc != NULL) *tc = data.tc;
 if (index != NULL) *index = data.index;
}

void AtomSet::SetArrays(long n, double ** pos, double ** vel, double ** acc, double ** aux0, Tag ** tc, long ** index)
{
 if (pos != NULL) this->pos = *pos;
 if (vel != NULL) this->vel = *vel;
 if (acc != NULL) this->acc = *acc;
 if (aux0 != NULL) this->aux0 = *aux0;
 if (tc != NULL) this->tc = *tc;
 if (index != NULL) this->index = *index;
 Resize(n);
 pos_size = sub_size = n;
}

bool AtomSet::HasReturnValue() const { return (rvalue_holder != 0); }

void AtomSet::PutReturnValue(const ReturnValue & r) const { rvalue_holder = const_cast<ReturnValue *>(&r); }

const ReturnValue & AtomSet::GetReturnValue() { return *rvalue_holder; }

void AtomSet::SetReturnType(int vtype) const { this->vtype = vtype; }

int AtomSet::GetReturnType() const { return vtype; }

void AtomSet::AddToLog(const char * msg) const
{
 log.push_back(msg);
}

}  // lpmd

