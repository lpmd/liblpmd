/*
 *
 *
 *
 */

#ifndef __LPMD_REGION_H__
#define __LPMD_REGION_H__

#include <lpmd/vector.h>

namespace lpmd
{

 class Region
 {
  public:
    virtual ~Region() { }

    virtual bool IsInside(const Vector & v) const = 0;

    virtual double Volume() const = 0;
 };

 //
 //
 //
 class Box: public Region
 {
  public:
    Box(double xlength, double ylength, double zlength)
    {
     for (int q=0;q<3;++q) vmin[q] = 0.0;
     vmax[0] = xlength;
     vmax[1] = ylength;
     vmax[2] = zlength;
    }
    
    Box(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, Vector _a, Vector _b, Vector _c)
    {
     vmin[0] = xmin;
     vmin[1] = ymin;
     vmin[2] = zmin;
     vmax[0] = xmax;
     vmax[1] = ymax;
     vmax[2] = zmax;
     a = _a ; b = _b ; c = _c;
    }

    inline bool IsInside(const Vector & v) const 
    {
     //Set the old coordinates based in the cell
     Vector vp(v[0]*a/a.Module() + v[1]*b/b.Module() + v[2]*c/c.Module());
     if ((fabs(vp[0]) < vmin[0]) || (fabs(vp[0]) > vmax[0])) return false;
     if ((fabs(vp[1]) < vmin[1]) || (fabs(vp[1]) > vmax[1])) return false;
     if ((fabs(vp[2]) < vmin[2]) || (fabs(vp[2]) > vmax[2])) return false;
     return true;
    }

    inline double Volume() const { return (vmax[0]-vmin[0])*(vmax[1]-vmin[1])*(vmax[2]-vmin[2]); }

  private:
   double vmin[3], vmax[3];
   Vector a,b,c;
 };

 class Sphere: public Region
 {
  public:
    Sphere(const Vector & center, double radius): _center(center), _radius(radius) { }
    
    inline bool IsInside(const Vector & v) const { return ((v-_center).Module() < _radius); }
    
    inline double Volume() const { return (4.0/3.0)*M_PI*pow(_radius, 3.0); }

  private:
    Vector _center;
    double _radius;
 };

 class Cylinder: public Region
 {
  public:
   Cylinder(const Vector & S, const Vector & origin, double rmax, double rmin): _S(S), _origin(origin), _rmax(rmax), _rmin(rmin) { }

   inline bool IsInside(const Vector & v) const 
   { 
    double temp = (Cross(v-_origin,_S).Module())/(_S.Module());
    if ( temp < _rmax && temp > _rmin)
    {
      Vector norm = _S ;
      norm.Normalize();
      double dp = Dot(norm, v-_origin);
      if (dp < 0 || dp > _S.Module()) return false;
      else return true;
    }
    else return false;
   }
   inline double Volume() const { return ((M_PI*(_rmax*_rmax-_rmin*_rmin))*_S.Module()); }
  private:
   Vector _S,_origin;
   double _rmax;
   double _rmin;
 };

 class Cone: public Region
 {
  public:
    Cone(const Vector & tip, const Vector & bot, double alpha, double beta): _tip(tip), _bot(bot), _alpha(alpha), _beta(beta) { }
    
    inline bool IsInside(const Vector & v) const 
    {
     Vector direct = _bot - _tip;
     Vector posrel = v - _tip;
     double dotab = Dot(direct,posrel);
     double angle = (acos(dotab/(direct.Module()*posrel.Module())))*180.0/M_PI;
     if(angle>=_alpha || angle<_beta) return false;
     if(posrel.Module()>direct.Module()) return false;
     else return true;
    }
    
    inline double Volume() const 
    { 
     double l = (_bot - _tip).Module();
     double A = (1.0e0/3.0e0)*l*pow(l*tan(_alpha),2);
     double B = (1.0e0/3.0e0)*l*pow(l*tan(_beta),2);
     return A-B; 
    }

  private:
    Vector _tip;
    Vector _bot;
    double _alpha,_beta;
 };



}  // lpmd

#endif

