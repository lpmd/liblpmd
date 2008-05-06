//
//
//

#include <lpmd/simulationcell.h>
#include <lpmd/cellmanager.h>
#include <lpmd/physunits.h>
#include <lpmd/util.h>
#include <lpmd/error.h>

#include "distcache.h"

#include <cmath>

using namespace lpmd;

//
//
CellManagerMissing::CellManagerMissing(): Error("No CellManager plugin enabled, cannot calculate neighbor lists!") { }

//
//
//
class lpmd::SimCellImpl
{
 public:
   unsigned int na, nb, nc;
   double virial;
   CellManager * cm;
   DistanceCache * dc;

   SimCellImpl(): na(1), nb(1), nc(1), cm(NULL), dc(NULL) { }
   SimCellImpl(int nra, int nrb, int nrc): na(nra), nb(nrb), nc(nrc), cm(NULL), dc(NULL) { }
   ~SimCellImpl() { if (dc != NULL) delete dc; }
};

SimulationCell::SimulationCell() { impl = new SimCellImpl();}

SimulationCell::SimulationCell(int nra, int nrb, int nrc, bool pba, bool pbb, bool pbc) 
{
 impl = new SimCellImpl(nra, nrb, nrc); 
 SetPeriodicity(0, pba);
 SetPeriodicity(1, pbb);
 SetPeriodicity(2, pbc);
}

SimulationCell::SimulationCell(const SimulationCell & sc): Particles(sc), Cell(sc)
{
 impl = new SimCellImpl(sc.impl->na, sc.impl->nb, sc.impl->nc);
 for (int i=0;i<3;++i) SetPeriodicity(i, sc.Periodic(i));
 if (sc.impl->dc != NULL) impl->dc = new DistanceCache(sc);
 //if (sc.impl->cm != NULL) SetCell(sc);
 SetCell(sc);
}

SimulationCell::~SimulationCell() { delete impl; }

SimulationCell & SimulationCell::operator=(const SimulationCell & sc)
{
 Particles::operator=(sc);
 delete impl;
 impl = new SimCellImpl(sc.impl->na, sc.impl->nb, sc.impl->nc);
 for (int i=0;i<3;++i) SetPeriodicity(i, sc.Periodic(i));
 if (sc.impl->dc != NULL) impl->dc = new DistanceCache(sc);
 //if (sc.impl->cm != NULL) SetCell(sc);
 SetCell(sc);
 return (*this);
}

void SimulationCell::RealPos()
{
 const long n = Size();
 for (int i=0;i<n;i++)
 {
  Atom tmp = GetAtom(i);
  Vector a = tmp.Position();
  tmp.SetPos(ScaleByCell(a));
  SetAtom(tmp, i);
 }
}

void SimulationCell::FracPos() 
{
 const long n = Size();
 for (int i=0;i<n;i++)
 {
  Vector a = GetAtom(i).Position(), n;
  ConvertToInternal(a);
  for (int j=0;j<3;++j) n.Set(j, a.Get(j)/GetVector(j).Mod());
  Atom tmp = GetAtom(i);
  tmp.SetPos(n);
  SetAtom(tmp, i);
 }
}

void SimulationCell::Center()
{
 const long n = Size();
 Vector displacement = ScaleByCell(Vector(0.5e0, 0.5e0, 0.5e0));
 for (long i=0;i<n;i++)
 {
  Atom tmp = GetAtom(i);
  tmp.SetPos(tmp.Position() - displacement);
  SetAtom(tmp, i);
 }
}

void SimulationCell::UnCenter()
{
 const long n = Size();
 Vector displacement = ScaleByCell(Vector(0.5e0, 0.5e0, 0.5e0));
 for (long i=0;i<n;i++)
 {
  Atom tmp = GetAtom(i);
  tmp.SetPos(tmp.Position() + displacement);
  SetAtom(tmp, i);
 }
}

void SimulationCell::SuperCell() { throw Error("SimulationCell::SuperCell() is obsolete! Please use the \"replicate\" plugin instead."); }

CellManager & SimulationCell::GetCellManager() const { return *(impl->cm); }

void SimulationCell::SetCellManager(CellManager & cellman) 
{ 
 cellman.Reset();
 impl->cm = &cellman;
 (impl->cm)->UpdateCell(*this);
}

void SimulationCell::UseDistanceCache(bool activ)
{
 if (activ)
 {
  if (impl->dc == NULL) impl->dc = new DistanceCache(*this);
 }
 else if (impl->dc != NULL) 
 {
  delete impl->dc;
  impl->dc = NULL;
 }
}

void SimulationCell::SetCell(const Cell & c)
{
 for (int i=0;i<3;++i) SetVector(i, c.GetVector(i));
}

void SimulationCell::SetPart(const Particles & p)
{
 for (int i=0;i<p.Size();i++) AppendAtom(p.GetAtom(i));
 NumEspec();
 if (impl->cm != NULL) (impl->cm)->UpdateCell(*this);
}

Vector SimulationCell::FracPosition(long i) const
{
 Vector a = GetAtom(i).Position(), n;
 ConvertToInternal(a);
 for (int j=0;j<3;++j) n.Set(j, a.Get(j)/GetVector(j).Mod());
 return n;
}

void SimulationCell::SetPosition(long i, const Vector & p)
{
 Vector vtmp = p;
 ConvertToInternal(vtmp);
 for (int j=0;j<3;++j)
 {
  double q = vtmp.Get(j);
  const double ll = GetVector(j).Mod();
  if (Periodic(j))
  {
   if (q < 0) q += ll*(1+floor(-q/ll)); 
   else if (q > ll) q -= ll*floor(q/ll);
   vtmp.Set(j, q);
  }
 }
 ConvertToExternal(vtmp);
 Atom a = GetAtom(i);
 a.SetIndex(i);
 a.SetPos(vtmp);
 SetAtom(a, i);
}

void SimulationCell::SetFracPosition(long i, const Vector & fp)
{
 SetPosition(i, ScaleByCell(fp));
}

void SimulationCell::SetVelocity(long i, const Vector & v)
{
 Atom a = GetAtom(i);
 a.SetVel(v);
 SetAtom(a, i);
}

void SimulationCell::SetAcceleration(long i, const Vector & ac)
{
 Atom a = GetAtom(i);
 a.SetAccel(ac);
 SetAtom(a, i);
}

void SimulationCell::ClearForces()
{
 const Vector zero;
 for (long i=0;i<Size();++i) SetAcceleration(i, zero);
 if (impl->dc != NULL) (impl->dc)->Clear();
 if (impl->cm != NULL) (impl->cm)->UpdateCell(*this);
 impl->virial = 0.0;
}

void SimulationCell::BuildNeighborList(long i, std::list<Neighbor> & nlist, bool full)
{
 if (impl->cm == NULL) throw CellManagerMissing();
 else (impl->cm)->BuildNeighborList(*this, i, nlist, full);
}


Vector SimulationCell::VectorDistance(long i, long j)
{
 if (impl->dc != NULL) return (impl->dc)->VectorDistance(i, j);
 return Displacement(Particles::GetAtom(i).Position(), Particles::GetAtom(j).Position());
}

/*
Vector SimulationCell::VectorDistance(const Atom & i, const Atom & j)
{
 return Displacement(i.Position(), j.Position());
}
*/

Vector SimulationCell::VectorRealDistance(long i, long j) { return GetAtom(i).Position() - GetAtom(j).Position(); }

Vector SimulationCell::VectorDistanceToReplica(long i, long j, long nx, long ny, long nz)
{
 if (impl->dc != NULL) return (impl->dc)->VectorDistanceToReplica(i, j, nx, ny, nz);
 Vector vi = Particles::GetAtom(i).Position();
 Vector vj = Particles::GetAtom(j).Position(); 
 if (nx==0 && (ny==0 && nz==0)) return vj-vi;  
 else return (ScaleByCell(Vector(nx, ny, nz)) + vj - vi);
}

double SimulationCell::Distance(long i, long j)
{
 if (impl->dc != NULL) return (impl->dc)->Distance(i, j);
 return VectorDistance(i, j).Mod();
}

/*
double SimulationCell::Distance(const Atom & i, const Atom & j)
{
 return Displacement(i.Position(), j.Position()).Mod();
}
*/

double SimulationCell::RealDistance(long i, long j) { return VectorRealDistance(i,j).Mod(); }

double SimulationCell::DistanceToReplica(long i, long j, long nx, long ny, long nz) { return VectorDistanceToReplica(i,j,nx,ny,nz).Mod(); }

double SimulationCell::Distance2(long i,long j)
{
 if (impl->dc != NULL) return (impl->dc)->Distance2(i, j);
 return VectorDistance(i, j).Mod2();
}

double SimulationCell::Angle(long i, long j, long k) 
{
 Vector a = VectorDistance(i,j);
 Vector b = VectorDistance(k,j);
 return acos(Dot(a, b) / (a.Mod()*b.Mod())); 
}

void SimulationCell::Rescale(double f)
{
 FracPos();
 for (int i=0;i<3;++i) Cell::Scale(i, f);
 RealPos();
}

void SimulationCell::Rescale(double f, int i)
{
 FracPos();
 Cell::Scale(i, f);
 RealPos();
}

void SimulationCell::RescalePercent(double p)
{
 FracPos();
 for (int i=0;i<3;++i) Cell::ScalePercent(i, p);
 RealPos();
}

void SimulationCell::RescalePercent(double p, int i)
{
 FracPos();
 Cell::ScalePercent(i,p);
 RealPos();
}

//
//
//
double SimulationCell::Virial() const { return impl->virial; }
//return ((1.0/3.0)*(KIN2EV*virial+virialcorrec)*PRESSFACTOR/v);

void SimulationCell::AddToVirial(double vir) { impl->virial += vir; }

//
//
//
double SimulationCell::Density() const
{
 double m = 0.0e0, v = Volume();
 for(long i=0;i<Size();++i)
 {
  Atom a = GetAtom(i);
  m += a.Mass();
 }
 return m/v;
}

//
//
//
double SimulationCell::ParticleDensity() const
{
 double N = (double)Size();
 double V = Volume();
 return N/V;
}

