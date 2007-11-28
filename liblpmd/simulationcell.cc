//
//
//

#include <cmath>

#include <lpmd/simulationcell.h>
#include <lpmd/physunits.h>
#include <lpmd/util.h>

using namespace lpmd;

SimulationCell::SimulationCell() 
{ 
 use_dc = false;
 distcache = NULL;
}

SimulationCell::SimulationCell(int nra, int nrb, int nrc, bool pba, bool pbb, bool pbc)
{
 use_dc = false;
 distcache = NULL;
 na = nra;
 nb = nrb;
 nc = nrc;
 pa = pba;
 pb = pbb;
 pc = pbc;
}

SimulationCell::~SimulationCell() 
{ 
 if (use_dc)
 {
  for (long i=0;i<Size();++i) delete [] distcache[i];
  delete [] distcache;
 }
}

void SimulationCell::RealPos()
{
 int N=Particles::Size();
 for (int i=0;i<N;i++)
 {
  Vector a = Particles::GetAtom(i).Position();
  Vector n = a.GetX()*Cell::GetVector(0) + a.GetY()*Cell::GetVector(1) + a.GetZ()*Cell::GetVector(2);
  Atom tmp = Particles::GetAtom(i);
  tmp.SetPos(n);
  Particles::SetAtom(tmp, i);
 }
}

void SimulationCell::FracPos() 
{
 int N = Particles::Size();
 for (int i=0;i<N;i++)
 {
  Vector a = Particles::GetAtom(i).Position();
  ConvertToInternal(a);
  Vector n(a.GetX()/Cell::GetVector(0).Mod(),a.GetY()/Cell::GetVector(1).Mod(),a.GetZ ()/Cell::GetVector(2).Mod());
  Atom tmp = Particles::GetAtom(i);
  tmp.SetPos(n);
  Particles::SetAtom(tmp, i);
 }
}

void SimulationCell::Center()
{
 int N = Particles::Size();
 Vector displacement = (Cell::GetVector(0) + Cell::GetVector(1) + Cell::GetVector(2))/2.0e0;
 for (int i=0;i<N;i++)
 {
  Vector a = Particles::GetAtom(i).Position();
  Vector n = a - displacement;
  Atom tmp = Particles::GetAtom(i);
  tmp.SetPos(n);
  Particles::SetAtom(tmp,i);
 }
}

void SimulationCell::UnCenter()
{
 int N = Particles::Size();
 Vector displacement = (Cell::GetVector(0) + Cell::GetVector(1) + Cell::GetVector(2))/2.0e0;
 for (int i=0;i<N;i++)
 {
  Vector a = Particles::GetAtom(i).Position();
  Vector n = a + displacement;
  Atom tmp = Particles::GetAtom(i);
  tmp.SetPos(n);
  Particles::SetAtom(tmp,i);
 }
}

void SimulationCell::SuperCell()
{
 int Ntmp = Particles::Size();
 Atom *atomos;
 atomos = new Atom[Ntmp];
 for(int i=0;i<Ntmp;i++) atomos[i]=Particles::GetAtom(i);
 Particles::Initialize(na*Ntmp);
 //int counter = 0;
 for(int i=0;i<Ntmp;i++){Particles::AppendAtom(atomos[i]);}//counter++;}

 for(unsigned int i=1;i<na;i++)
 {
  for(int j=0;j<Ntmp;j++)
  {
   Atom tmp=atomos[j];
   tmp.SetPos(atomos[j].Position()+i*Cell::GetVector(0));
   Particles::AppendAtom(tmp);
  }
 }
 delete[] atomos;
 Ntmp = Particles::Size();
 atomos = new Atom[Ntmp];
 for(int i=0;i<Ntmp;i++) atomos[i]=Particles::GetAtom(i);
 Particles::Initialize(nb*Ntmp);
 for(int i=0;i<Ntmp;i++){Particles::AppendAtom(atomos[i]);}
 for(unsigned int i=1;i<nb;i++)
 {
  for(int j=0;j<Ntmp;j++)
  {
   Atom tmp=atomos[j];
   tmp.SetPos(atomos[j].Position()+i*Cell::GetVector(1));
   Particles::AppendAtom(tmp);
  }
 }
 delete[] atomos;
 Ntmp = Particles::Size();
 atomos = new Atom[Ntmp];
 for(int i=0;i<Ntmp;i++) atomos[i]=Particles::GetAtom(i);
 Particles::Initialize(nc*Ntmp);
 for(int i=0;i<Ntmp;i++) {Particles::AppendAtom(atomos[i]);}
 for(unsigned int i=1;i<nc;i++)
 {
  for(int j=0;j<Ntmp;j++)
  {
   Atom tmp=atomos[j];
   tmp.SetPos(atomos[j].Position()+i*Cell::GetVector(2));
   Particles::AppendAtom(tmp);
  }
 }
 delete[]atomos;
 //Resetea los vectores base de la celda.
 Vector a=Cell::GetVector(0);
 Cell::SetVector(0,na*a);
 Vector b=Cell::GetVector(1);
 Cell::SetVector(1,nb*b);
 Vector c=Cell::GetVector(2);
 Cell::SetVector(2,nc*c);
}

void SimulationCell::UseDistanceCache(bool activ) 
{ 
 use_dc = activ; 
 if ((use_dc) && (distcache == NULL))
 {
  distcache = new DistCacheItem*[Size()];
  for (long i=0;i<Size();++i) distcache[i] = new DistCacheItem[Size()];
  ClearDistanceCache();
 }
}

void SimulationCell::ClearDistanceCache()
{
 if (use_dc)
 {
  for (long i=0;i<Size();++i) 
    for (long j=0;j<Size();++j) distcache[i][j].active = false;
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
}

Vector SimulationCell::FracPosition(long i) const
{
 Vector a = GetAtom(i).Position();
 ConvertToInternal(a);
 Vector n(a.GetX()/GetVector(0).Mod(), a.GetY()/GetVector(1).Mod(), a.GetZ()/GetVector(2).Mod());
 return n;
}

void SimulationCell::SetPosition(long i, const Vector & p)
{
 Vector vtmp = p;
 ConvertToInternal(vtmp);
 double x = vtmp.GetX();
 double y = vtmp.GetY();
 double z = vtmp.GetZ();
 const double Lx = GetVector(0).Mod();
 const double Ly = GetVector(1).Mod();
 const double Lz = GetVector(2).Mod();
 if (pa == true)
 {
  if (x < 0) x += Lx*(1+floor(-x/Lx)); 
  else if (x > Lx) x -= Lx*floor(x/Lx);
 }
 if (pb == true)
 {
  if (y < 0) y += Ly*(1+floor(-y/Ly));
  else if (y > Ly) y -= Ly*floor(y/Ly);
 }
 if (pc == true)
 {
  if (z < 0) z += Lz*(1+floor(-z/Lz));
  else if (z > Lz) z -= Lz*floor(z/Lz);
 }
 Atom a = GetAtom(i);
 vtmp = Vector(x, y, z);
 ConvertToExternal(vtmp);
 a.SetPos(vtmp);
 SetAtom(a, i);
}

void SimulationCell::SetFracPosition(long i, const Vector & fp)
{
 Vector p;
 p = GetVector(0)*fp.GetX() + GetVector(1)*fp.GetY() + GetVector(2)*fp.GetZ();
 SetPosition(i, p);
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
 for (long i=0;i<Size();++i) SetAcceleration(i, Vector(0.0e0));
 ClearDistanceCache();
}

Vector SimulationCell::Displacement(const Vector & a, const Vector & b) const
{
 Vector d = b - a;
 ConvertToInternal(d);
 double x = d.GetX();
 double y = d.GetY();
 double z = d.GetZ();
 if (pa == true)
 {
  double A = GetVector(0).Mod();
  if (x >= 0.5*A) x = x - A;
  else if (x < -0.5*A) x = x + A;
 }
 if (pb == true)
 {
  double B = GetVector(1).Mod();
  if (y >= 0.5*B) y = y - B;
  else if (y <- 0.5*B) y = y + B;
 }
 if (pc == true)
 {
  double C = GetVector(2).Mod();
  if (z >= 0.5*C) z = z - C;
  else if (z <- 0.5*C) z = z + C;
 }
 d = Vector(x, y, z);
 ConvertToExternal(d);
 return d;
}

Vector SimulationCell::VectorDistance(long i, long j)
{
 if (! use_dc) return Displacement(Particles::GetAtom(i).Position(), Particles::GetAtom(j).Position());
 // Consulta al cache de distancias
 DistCacheItem & p = distcache[j][i];
 if (p.active == true) return p.d;
 else
 {
  const Vector & vd = Displacement(Particles::GetAtom(i).Position(), Particles::GetAtom(j).Position());
  distcache[j][i].active = true;
  distcache[j][i].d = vd;
  double r = vd.Mod();
  distcache[j][i].r = r;
  distcache[j][i].r2 = r*r;
  return vd;
 }
}

Vector SimulationCell::VectorRealDistance(long i, long j)
{
 return Particles::GetAtom(i).Position() - Particles::GetAtom(j).Position();
}

Vector SimulationCell::VectorDistanceToReplica(long i, long j, long nx, long ny, long nz) const
{
 Vector vi = Particles::GetAtom(i).Position();
 Vector vj = Particles::GetAtom(j).Position(); 
 if(nx==0 && (ny==0 && nz==0)) return vj-vi;  
 else    
 {
  vj = (Cell::GetVector(0)*nx+Cell::GetVector(1)*ny+Cell::GetVector(2)*nz)+vj;
  return vj-vi;    
 }   
}

double SimulationCell::Distance(long i, long j)
{
 if (! use_dc) return VectorDistance(i, j).Mod();
 // Consulta al cache de distancias
 DistCacheItem & p = distcache[j][i];
 if (p.active == true) return p.r;
 else
 {
  const Vector & vd = Displacement(Particles::GetAtom(i).Position(), Particles::GetAtom(j).Position());
  distcache[j][i].active = true;
  distcache[j][i].d = vd;
  double r = vd.Mod();
  distcache[j][i].r = r;
  distcache[j][i].r2 = r*r;
  return r;
 }
}

double SimulationCell::RealDistance(long i, long j)
{
 return VectorRealDistance(i,j).Mod();
}

double SimulationCell::DistanceToReplica(long i, long j, long nx, long ny, long nz) const
{
 return VectorDistanceToReplica(i,j,nx,ny,nz).Mod();
}

double SimulationCell::Distance2(long i,long j)
{
 if (! use_dc) return VectorDistance(i, j).Mod2();
 // Consulta al cache de distancias
 DistCacheItem & p = distcache[j][i];
 if (p.active == true) return p.r2;
 else
 {
  const Vector & vd = Displacement(Particles::GetAtom(i).Position(), Particles::GetAtom(j).Position());
  distcache[j][i].active = true;
  distcache[j][i].d = vd;
  double r = vd.Mod();
  double r2 = r*r;
  distcache[j][i].r = r;
  distcache[j][i].r2 = r2;
  return r2;
 }
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
 Cell::Scale(0, f);
 Cell::Scale(1, f);
 Cell::Scale(2, f);
 RealPos();
}

void SimulationCell::Rescale(double f, int i)
{
 FracPos();
 Cell::Scale(i,f);
 RealPos();
}

void SimulationCell::RescalePercent(double p)
{
 FracPos();
 Cell::ScalePercent(0, p);
 Cell::ScalePercent(1, p);
 Cell::ScalePercent(2, p);
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
double SimulationCell::Pressure() const
{
 double v = Volume();
 double virial = 0.0e0;
 for (long i=0;i<Size();++i)
 {
  Atom a = GetAtom(i);
  double m = a.Mass();
  Vector ff = a.Acceleration()*m;
  virial += (Dot(a.Position(), ff));
 }
 return ((1.0/3.0)*virial*KIN2EV + double(Size())*KBOLTZMANN*Temperature())*PRESSFACTOR / v;
}


