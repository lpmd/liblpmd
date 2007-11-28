//
//
//

#include <cmath>

#include <lpmd/cell.h>

using namespace lpmd;

Cell::Cell() { }

Cell::Cell(Vector a, Vector b, Vector c) 
{ 
 v[0]=a; 
 v[1]=b; 
 v[2]=c; 
 UpdateTransfMatrix();
}

Cell::Cell(double a, double b, double c, double alpha, double beta, double gamma)
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
 UpdateTransfMatrix();
}

Cell::~Cell() { }

void Cell::UpdateTransfMatrix()
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

void Cell::ReSet(double a,double b,double c,double alpha,double beta,double gamma)
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
 UpdateTransfMatrix();
} 

void Cell::ConvertToInternal(Vector & v) const
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

void Cell::ConvertToExternal(Vector & v) const
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

double Cell::Volume() const
{
 Vector tmp = Crux(v[0], v[1]);
 return fabs(Dot(tmp, v[2]));
}

void Cell::Scale(int i, double f) 
{ 
 v[i] = v[i]*f; 
 UpdateTransfMatrix();
}

void Cell::ScalePercent(int i, double p) 
{ 
 v[i] *= (1.0e0 + p/100.0e0); 
 UpdateTransfMatrix();
}

std::ostream & operator<<(std::ostream & os, const Cell & c)
{
 os << c.GetVector(0) << " " << c.GetVector(1) << " " << c.GetVector(2) << '\n';
 return os;
}



