//
//
//

#include <lpmd/cell.h>
#include <lpmd/vector.h>

#include <cmath>
#include <iostream>

using namespace lpmd;

class lpmd::CellImpl
{
 public:
   Vector v[3];
   bool p[3];
   double tm[3][3], itm[3][3];
   double nonortg;

  CellImpl() 
  { 
   v[0] = Vector(1.0, 0.0, 0.0);
   v[1] = Vector(0.0, 1.0, 0.0);
   v[2] = Vector(0.0, 0.0, 1.0);
   for (int i=0;i<3;++i) p[i] = true;
   UpdateTransfMatrix();
  }

  CellImpl(const Vector & a, const Vector & b, const Vector & c)
  {
   v[0] = a; 
   v[1] = b; 
   v[2] = c; 
   for (int i=0;i<3;++i) p[i] = true;
   UpdateTransfMatrix();
  }

  CellImpl(double a, double b, double c, double alpha, double beta, double gamma)
  {
   v[0].SetX(a);
   v[0].SetY(0.0e0);
   v[0].SetZ(0.0e0);
   v[1].SetX(b*cos(gamma));
   v[1].SetY(b*sin(gamma));
   v[1].SetZ(0.0e0);
   v[2].SetX(c*cos(beta));
   double tmp=(cos(alpha)-cos(gamma)*cos(beta))/sin(gamma);
   v[2].SetY(c*tmp);
   v[2].SetZ(c*sqrt(sin(beta)*sin(beta)-tmp*tmp));
   for (int i=0;i<3;++i) p[i] = true;
   UpdateTransfMatrix();
  }

  void UpdateTransfMatrix()
  {
   Vector vnorm[3];
   for (int i=0;i<3;++i) 
   {
    vnorm[i] = v[i];
    vnorm[i].Norm();
   }
   tm[0][0] = vnorm[1].Get(1)*vnorm[2].Get(2)-vnorm[2].Get(1)*vnorm[1].Get(2);
   tm[1][0] = vnorm[2].Get(0)*vnorm[1].Get(2)-vnorm[1].Get(0)*vnorm[2].Get(2);
   tm[2][0] = vnorm[1].Get(0)*vnorm[2].Get(1)-vnorm[2].Get(0)*vnorm[1].Get(1);
   tm[0][1] = vnorm[2].Get(1)*vnorm[0].Get(2)-vnorm[0].Get(1)*vnorm[2].Get(2);
   tm[1][1] = vnorm[0].Get(0)*vnorm[2].Get(2)-vnorm[2].Get(0)*vnorm[0].Get(2);
   tm[2][1] = vnorm[2].Get(0)*vnorm[0].Get(1)-vnorm[0].Get(0)*vnorm[2].Get(1);
   tm[0][2] = vnorm[0].Get(1)*vnorm[1].Get(2)-vnorm[1].Get(1)*vnorm[0].Get(2);
   tm[1][2] = vnorm[1].Get(0)*vnorm[0].Get(2)-vnorm[0].Get(0)*vnorm[1].Get(2);
   tm[2][2] = vnorm[0].Get(0)*vnorm[1].Get(1)-vnorm[1].Get(0)*vnorm[0].Get(1);
   const double d = tm[0][0]*vnorm[0].Get(0) + tm[0][1]*vnorm[1].Get(0) + tm[0][2]*vnorm[2].Get(0);
   nonortg = 0.0;
   double x;
   for (int j=0;j<3;++j)
     for (int i=0;i<3;++i)
     {
      itm[j][i] = vnorm[j].Get(i);
      tm[j][i] = (tm[j][i] / d);
      x = ((i == j) ? 1.0 : 0.0);
      nonortg += ((x-tm[j][i])*(x-tm[j][i]));
     }
   nonortg = sqrt(nonortg);
  }

 void ConvertToInternal(Vector & v) const
 {
  if (nonortg > 1.0e-10)
  {
   const double x = v.Get(0);
   const double y = v.Get(1);
   const double z = v.Get(2);
   v.Set(0, x*tm[0][0]+y*tm[1][0]+z*tm[2][0]);
   v.Set(1, x*tm[0][1]+y*tm[1][1]+z*tm[2][1]);
   v.Set(2, x*tm[0][2]+y*tm[1][2]+z*tm[2][2]);
  }
 }

 void ConvertToExternal(Vector & v) const
 {
  if (nonortg > 1.0e-10)
  {
   const double x = v.Get(0);
   const double y = v.Get(1);
   const double z = v.Get(2);
   v.Set(0, x*itm[0][0]+y*itm[1][0]+z*itm[2][0]);
   v.Set(1, x*itm[0][1]+y*itm[1][1]+z*itm[2][1]);
   v.Set(2, x*itm[0][2]+y*itm[1][2]+z*itm[2][2]);
  }
 }

};

Cell::Cell() { impl = new CellImpl(); }

Cell::Cell(const Vector & a, const Vector & b, const Vector & c) { impl = new CellImpl(a, b, c); }

Cell::Cell(double a, double b, double c, double alpha, double beta, double gamma) { impl = new CellImpl(a, b, c, alpha, beta, gamma); }

Cell::Cell(const Cell & c)
{
 impl = new CellImpl(c.GetVector(0), c.GetVector(1), c.GetVector(2));
 UpdateTransfMatrix();
}

Cell::~Cell() { delete impl; }

Cell & Cell::operator=(const Cell & c)
{
 if (&c != this) 
 {
  delete impl;
  impl = new CellImpl(c.GetVector(0), c.GetVector(1), c.GetVector(2));
  UpdateTransfMatrix();
 }
 return (*this);
}

Vector Cell::GetVector(int i) const { return impl->v[i]; }

void Cell::SetVector(int i, Vector a) 
{ 
 impl->v[i] = a; 
 UpdateTransfMatrix(); 
}

void Cell::SetPeriodicity(int axis, bool value) { impl->p[axis] = value; }

bool Cell::Periodic(int axis) const { return impl->p[axis]; }

void Cell::UpdateTransfMatrix() { impl->UpdateTransfMatrix(); }

void Cell::ReSet(double a, double b, double c, double alpha, double beta, double gamma)
{ 
 impl->v[0].SetX(a);
 impl->v[0].SetY(0.0e0);
 impl->v[0].SetZ(0.0e0);
 impl->v[1].SetX(b*cos(gamma));
 impl->v[1].SetY(b*sin(gamma));
 impl->v[1].SetZ(0.0e0);
 impl->v[2].SetX(c*cos(beta));
 double tmp=(cos(alpha)-cos(gamma)*cos(beta))/sin(gamma);
 impl->v[2].SetY(c*tmp);
 impl->v[2].SetZ(c*sqrt(sin(beta)*sin(beta)-tmp*tmp));
 UpdateTransfMatrix();
} 

Vector Cell::ScaleByCell(const Vector & cv) const
{
 Vector nv;
 for (int j=0;j<3;++j) nv += (impl->v[j])*cv.Get(j);
 return nv;
}

void Cell::ConvertToInternal(Vector & v) const { impl->ConvertToInternal(v); }

void Cell::ConvertToExternal(Vector & v) const { impl->ConvertToExternal(v); }

double Cell::Volume() const
{
 Vector tmp = Crux(impl->v[0], impl->v[1]);
 return fabs(Dot(tmp, impl->v[2]));
}

Vector Cell::Displacement(const Vector & a, const Vector & b) const
{
 Vector d = b - a;
 ConvertToInternal(d);
 for (int i=0;i<3;++i)
 {
  double q = d.Get(i);
  if (Periodic(i))
  {
   double ll = GetVector(i).Mod();
   if (q >= 0.5*ll) q = q - ll;
   else if (q < -0.5*ll) q = q + ll;
   d.Set(i, q);
  }
 }
 ConvertToExternal(d);
 return d;
}

void Cell::Scale(int i, double f) 
{ 
 impl->v[i] = impl->v[i]*f; 
 UpdateTransfMatrix();
}

void Cell::ScalePercent(int i, double p) 
{ 
 impl->v[i] *= (1.0e0 + p/100.0e0); 
 UpdateTransfMatrix();
}

std::ostream & operator<<(std::ostream & os, const Cell & c)
{
 os << c.GetVector(0) << " " << c.GetVector(1) << " " << c.GetVector(2) << '\n';
 return os;
}


