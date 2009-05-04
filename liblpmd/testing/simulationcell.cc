//
//
//

#include <lpmd/simulationcell.h>
#include <lpmd/cellmanager.h>
#include <lpmd/util.h>
#include <lpmd/error.h>
#include <lpmd/session.h>

#include <cmath>

using namespace lpmd;

//
//
CellManagerMissing::CellManagerMissing(): Error("No CellManager plugin enabled, cannot calculate neighbor lists!") { }

//
//
//
class lpmd::SimCellImpl: public Cell
{
 public:
   unsigned int na, nb, nc;
   double virial;
   double s[3][3];
   CellManager * cm;
   ParamList mdata;

   SimCellImpl(): na(1), nb(1), nc(1), cm(NULL) { }
   SimCellImpl(int nra, int nrb, int nrc): na(nra), nb(nrb), nc(nrc), cm(NULL) { }
   ~SimCellImpl() { }

   lpmd::SimCellImpl & operator=(const lpmd::SimCellImpl & ci)
   {
    if (this != &ci)
    {
     na = ci.na;
     nb = ci.nb;
     nc = ci.nc;
     virial = ci.virial;
     for (int i=0;i<3;++i)
       for (int j=0;j<3;++j) s[i][j] = ci.s[i][j];
     cm = ci.cm;    // FIXME: deberia clonar el CellManager???
     mdata = ci.mdata;
    }
    return (*this);
   }

};

SimulationCell::SimulationCell() { impl = new SimCellImpl();}

SimulationCell::SimulationCell(int nra, int nrb, int nrc, bool pba, bool pbb, bool pbc) 
{
 impl = new SimCellImpl(nra, nrb, nrc); 
 Cell & ic = (*impl);
 ic.Periodicity(0) = pba;
 ic.Periodicity(1) = pbb;
 ic.Periodicity(2) = pbc;
 for (int i=0;i<3;i++) 
 {
  for (int j=0;j<3;j++) impl->s[i][j]=0.0e0;
 }
}

SimulationCell::SimulationCell(const SimulationCell & sc): ParticleSet(sc)
{
 impl = new SimCellImpl(sc.impl->na, sc.impl->nb, sc.impl->nc);
 Cell & ic = (*impl);
 impl->operator=(*(sc.impl));
 for (int i=0;i<3;++i) ic.Periodicity(i) = sc.Periodicity(i);
 SetCell(ic);
}

SimulationCell::~SimulationCell() { delete impl; }

SimulationCell & SimulationCell::operator=(const SimulationCell & sc)
{
 ParticleSet::operator=(sc);
 delete impl;
 impl = new SimCellImpl(sc.impl->na, sc.impl->nb, sc.impl->nc);
 Cell & ic = (*impl);
 for (int i=0;i<3;++i) ic.Periodicity(i) = sc.Periodicity(i);
 SetCell(ic);
 return (*this);
}

Cell & SimulationCell::GetCell() const { return (*impl); }   // FIXME: que nombre ponerle?

void SimulationCell::SetCell(const Cell & c) 
{ 
 for (int q=0;q<3;++q) (*impl)[q] = c[q];
}

void SimulationCell::RealPos()
{
 //FIXME : chequear
 for (unsigned long int i=0;i<size();i++)
 {
  const Vector & a = operator[](i).Position();
  operator[](i).SetPos(ScaleByCell(a));
 }
}

void SimulationCell::FracPos() 
{
 Cell & ic = (*impl);
 for (unsigned long int i=0;i<size();i++)
 {
  Vector n, a = operator[](i).Position();
  ConvertToInternal(a);
  for (int j=0;j<3;++j) n[j] = a[j]/ic[j].Module();
  operator[](i).SetPos(n);
 }
}

void SimulationCell::Center()
{
 // FIXME: chequear
 Vector displacement = ScaleByCell(Vector(0.5e0, 0.5e0, 0.5e0));
 for (unsigned long int i=0;i<size();i++)
 {
  operator[](i).SetPos(operator[](i).Position()-displacement);
 }
}

void SimulationCell::UnCenter()
{
 // FIXME: chequear
 Vector displacement = ScaleByCell(Vector(0.5e0, 0.5e0, 0.5e0));
 for (unsigned long int i=0;i<size();i++)
 {
  operator[](i).SetPos(operator[](i).Position()+displacement);
 }
}

CellManager & SimulationCell::GetCellManager() const { return *(impl->cm); }

void SimulationCell::SetCellManager(CellManager & cellman) 
{ 
 cellman.Reset();
 impl->cm = &cellman;
 (impl->cm)->UpdateCell(*this);
}

void SimulationCell::SetPart(const ParticleSet & p)
{
 for (unsigned long int i=0;i<p.size();i++) Create(new Atom(p[i]));
 NumEspec();
 if (impl->cm != NULL) (impl->cm)->UpdateCell(*this);
}

Vector SimulationCell::FracPosition(long i) const
{
 Cell & ic = (*impl);
 Vector a = operator[](i).Position(), n;
 ConvertToInternal(a);
 for (int j=0;j<3;++j) n[j] = a[j]/ic[j].Module();
 return n;
}

void SimulationCell::SetPosition(long i, const Vector & p)
{
 Cell & ic = (*impl);
 Vector vtmp = p;
 ConvertToInternal(vtmp);
 for (int j=0;j<3;++j)
 {
  double q = vtmp[j];
  const double ll = ic[j].Module();
  if (ic.Periodicity(j))
  {
   if (q < 0) q += ll*(1+floor(-q/ll)); 
   else if (q > ll) q -= ll*floor(q/ll);
   vtmp[j] = q;
  }
 }
 ConvertToExternal(vtmp);
 operator[](i).SetIndex(i);
 operator[](i).SetPos(vtmp);
}

void SimulationCell::SetFracPosition(long i, const Vector & fp)
{
 SetPosition(i, ScaleByCell(fp));
}

void SimulationCell::SetVelocity(long i, const Vector & v) { operator[](i).SetVel(v); }

void SimulationCell::SetAcceleration(long i, const Vector & ac) { operator[](i).SetAccel(ac); }

void SimulationCell::SetColor(long i, const Vector & c) {operator[](i).SetColor(c);}

void SimulationCell::SetColor(long i, const double & c) {operator[](i).SetColor(c);}

void SimulationCell::ClearForces()
{
 const Vector zero;
 for (unsigned long int i=0;i<size();++i) SetAcceleration(i, zero);
 if (impl->cm != NULL) (impl->cm)->UpdateCell(*this);
 impl->virial = 0.0;
}

void SimulationCell::BuildNeighborList(long i, std::vector<Neighbor> & nlist, bool full, double rcut)
{
 if (impl->cm == NULL) throw CellManagerMissing();
 else (impl->cm)->BuildNeighborList(*this, i, nlist, full, rcut);
}

Vector SimulationCell::VectorDistance(long i, long j)
{
 return Displacement(operator[](i).Position(), operator[](j).Position());
}

Vector SimulationCell::VectorRealDistance(long i, long j) { return operator[](i).Position() - operator[](j).Position(); }

Vector SimulationCell::VectorDistanceToReplica(long i, long j, long nx, long ny, long nz)
{
 const Vector & vi = operator[](i).Position();
 const Vector & vj = operator[](j).Position(); 
 if (nx==0 && (ny==0 && nz==0)) return vj-vi;  
 else { return (ScaleByCell(Vector(nx, ny, nz)) + vj - vi); } // FIXME: chequear
}

double SimulationCell::Distance(long i, long j)
{
 return VectorDistance(i, j).Module();
}

double SimulationCell::RealDistance(long i, long j) { return VectorRealDistance(i,j).Module(); }

double SimulationCell::DistanceToReplica(long i, long j, long nx, long ny, long nz) { return VectorDistanceToReplica(i,j,nx,ny,nz).Module(); }

double SimulationCell::Distance2(long i,long j)
{
 return VectorDistance(i, j).SquareModule();
}

double SimulationCell::Angle(long i, long j, long k) 
{
 Vector a = VectorDistance(i,j);
 Vector b = VectorDistance(k,j);
 return acos(Dot(a, b) / (a.Module()*b.Module())); 
}

// Implementacion de la interfaz BasicCell
const bool & SimulationCell::Periodicity(int q) const { return impl->Periodicity(q); }

bool & SimulationCell::Periodicity(int q) { return impl->Periodicity(q); }

Vector SimulationCell::ScaleByCell(const BasicVector & cv) const { return impl->ScaleByCell(cv); }

void SimulationCell::ConvertToExternal(BasicVector & v) const { impl->ConvertToExternal(v); }

void SimulationCell::ConvertToInternal(BasicVector & v) const { impl->ConvertToInternal(v); }

Vector SimulationCell::Displacement(const Vector & a, const Vector & b) const { return impl->Displacement(a, b); }

double SimulationCell::Volume() const { return impl->Volume(); }

void SimulationCell::Rescale(double f)
{
 Cell & ic = (*impl);
 FracPos();
 for (int i=0;i<3;++i) ic[i] *= f; // FIXME: chequear
 RealPos();
}

void SimulationCell::Rescale(double f, int i)
{
 Cell & ic = (*impl);
 FracPos();
 ic[i] *= f; // FIXME: chequear
 RealPos();
}

void SimulationCell::RescalePercent(double p)
{
 Cell & ic = (*impl);
 FracPos();
 for (int i=0;i<3;++i) ic[i] *= (1+p/100.0); // FIXME: chequear
 RealPos();
}

void SimulationCell::RescalePercent(double p, int i)
{
 Cell & ic = (*impl);
 FracPos();
 ic[i] *= (1+p/100.0); // FIXME: chequear
 RealPos();
}

void SimulationCell::RescaleVector(Vector sx, Vector sy, Vector sz)
{
 Cell & ic = (*impl);
 FracPos();
 ic[0] += sx; // FIXME: chequear, originalmente Cell::ScaleVector(0, sx)
 ic[1] += sy;
 ic[2] += sz;
 RealPos();
}

void SimulationCell::RescaleVector(Vector s, int i)
{
 Cell & ic = (*impl);
 FracPos();
 ic[i] += s; // FIXME: chequear, originalmente Cell:ScaleVector(i,s)
 RealPos();
}

//
// SECCION :
// Calculo y/o asignacion de Propiedades Fisicas para un
// SimulationCell
//

double SimulationCell::Virial() const { return impl->virial; }
void SimulationCell::AddToVirial(double vir) { impl->virial += vir; }

double SimulationCell::Density() const
{
 double m = 0.0e0, v = Volume();
 for (unsigned long int i=0;i<size();++i) m += operator[](i).Mass();
 return GlobalSession.GetDouble("ua3togrcm3")*m/v;
}

double SimulationCell::ParticleDensity() const
{
 double N = double(size());
 double V = Volume();
 return N/V;
}

double & SimulationCell::StressTensor(int alpha, int beta) //Combina Get y Set
{
 if(alpha>2 || alpha<0) 
 {
  throw Error("bad alpha index for stress set/get, please report");
 }
 else if(beta>2 || beta<0)
 {
  throw Error("bad beta index for stress set/get, please report");
 }
 else return impl->s[alpha][beta];
}

ParamList & SimulationCell::MetaData() const { return impl->mdata; }

double SimulationCell::CMCutoff() const
{
 if (impl->cm == NULL) throw CellManagerMissing();
 CellManager & cm = GetCellManager();
 return cm.Cutoff();
}

