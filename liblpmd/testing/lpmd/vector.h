//
//
//

#ifndef __LPMD_VECTOR_H__
#define __LPMD_VECTOR_H__

#include <lpmd/util.h>

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdlib>

namespace lpmd
{

class Vector
{
 public:
   Vector();
   Vector(double x, double y, double z);
   Vector(const Vector & c);
   Vector(const std::string & s);
   ~Vector();
 
   void SetX(double x);
   void SetY(double y);
   void SetZ(double z);

   void Set(int i, double v);
   void Set(double x, double y, double z);

   const double & Get(int i) const;
   const double & GetX() const;
   const double & GetY() const;
   const double & GetZ() const;

   double Mod() const;
   double Mod2() const;
   void Scale(double f);
   void Norm();
   void Zero();

   Vector& operator=(const Vector & A);
   
   const Vector & operator+=(const Vector & a);
   const Vector & operator-=(const Vector & a);
   const Vector & operator*=(const double a);
   const Vector & operator/=(const double a);

   friend Vector operator+(const Vector &A, const Vector &B);
   friend Vector operator+(const Vector &A, const double &a);
   friend Vector operator-(const Vector &A, const Vector &B);
   friend Vector operator-(const Vector &A, const double &a);
   friend Vector operator*(const Vector &A, const Vector &B);
   friend Vector operator*(const Vector &A, const double &a);
   friend Vector operator*(const double &a, const Vector &A);
   friend Vector operator/(const Vector &A, const double &a);
   friend Vector operator/(const Vector &A, const Vector &B);

private:
   double q[3];
};

//
//
//

inline Vector::Vector() { q[0]=0.0e0; q[1]=0.0e0; q[2]=0.0e0; }

inline Vector::Vector(double x, double y, double z) { q[0]=x; q[1]=y; q[2]=z; }

inline Vector::Vector(const Vector & c) { q[0]=c.q[0]; q[1]=c.q[1]; q[2]=c.q[2]; }

inline Vector::Vector(const std::string & s)
{
 std::string tmps(s);
 if (tmps.size()<7) {std::cerr << "[Warning] Setting a vector from a string less than 7 characters long" << '\n';}
 tmps.erase(0,1);
 tmps.erase(tmps.size()-1,1);
 std::vector<std::string> v = SplitTextLine(tmps,',');
 if (v.size()!=3) {std::cerr << "[Warning] Setting a vector from a string different from 3 numbers!!! " << '\n';}
 SetX(atof(v[0].c_str()));
 SetY(atof(v[1].c_str()));
 SetZ(atof(v[2].c_str()));
}

inline Vector::~Vector() { }

inline void Vector::SetX(double x) { q[0]=x; }

inline void Vector::SetY(double y) { q[1]=y; }

inline void Vector::SetZ(double z) { q[2]=z; }

inline void Vector::Set(int i, double v) { q[i]=v; }

inline void Vector::Set(double x, double y, double z) { q[0]=x; q[1]=y; q[2]=z; }

inline const double & Vector::Get(int i) const { return q[i]; }

inline const double & Vector::GetX() const { return q[0]; }

inline const double & Vector::GetY() const { return q[1]; }

inline const double & Vector::GetZ() const { return q[2]; }

inline double Vector::Mod() const { return sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]); }

inline double Vector::Mod2() const { return q[0]*q[0]+q[1]*q[1]+q[2]*q[2]; }

inline void Vector::Scale(double f) { for (int i=0;i<3;++i) q[i] = q[i]*f; }

inline void Vector::Norm()
{
 double m=Mod();
 q[0]=q[0]/m;
 q[1]=q[1]/m;
 q[2]=q[2]/m;
}

inline void Vector::Zero() 
{ 
 q[0]=0.0e0;
 q[1]=0.0e0;
 q[2]=0.0e0;
}

inline Vector operator+(const Vector &A, const Vector &B)
{
 return Vector(A.q[0]+B.q[0],A.q[1]+B.q[1],A.q[2]+B.q[2]);
}

inline Vector operator+(const Vector &A, const double &a)
{
 return Vector(A.q[0]+a,A.q[1]+a,A.q[2]+a);
}

inline Vector operator-(const Vector &A, const Vector &B)
{
 return Vector(A.q[0]-B.q[0],A.q[1]-B.q[1],A.q[2]-B.q[2]);
}

inline Vector operator-(const Vector &A, const double &a)
{
 return Vector(A.q[0]-a,A.q[1]-a,A.q[2]-a);
}

inline Vector operator*(const Vector &A, const Vector &B)
{
 return Vector(A.q[0]*B.q[0],A.q[1]*B.q[1],A.q[2]*B.q[2]);
}

inline Vector operator*(const Vector &A, const double &a)
{
 return Vector(A.q[0]*a,A.q[1]*a,A.q[2]*a);
}

inline Vector operator*(const double &a, const Vector &A)
{
 return Vector(A.q[0]*a,A.q[1]*a,A.q[2]*a);
}

inline Vector operator/(const Vector &A, const double &a)
{
 return Vector(A.q[0]/a,A.q[1]/a,A.q[2]/a);
}

inline Vector operator/(const Vector &A, const Vector &B)
{
 return Vector(A.q[0]/B.q[0],A.q[1]/B.q[1],A.q[2]/B.q[2]);
}

inline const Vector & Vector::operator+=(const Vector & a)
{
 for (int j=0;j<3;++j) q[j] += a.q[j];
 return (*this);
}

inline const Vector & Vector::operator-=(const Vector & a)
{
 for (int j=0;j<3;++j) q[j] -= a.q[j];
 return (*this);
}

inline const Vector & Vector::operator*=(const double a)
{
 for (int j=0;j<3;++j) q[j] *= a; 
 return (*this); 
}

inline const Vector & Vector::operator/=(const double a)
{
 for (int j=0;j<3;++j) q[j] /= a;
 return (*this); 
}

inline Vector & Vector::operator=(const Vector & A)
{ 
 q[0]=A.q[0];
 q[1]=A.q[1];
 q[2]=A.q[2];
 return(*this);
}

inline int operator==(const Vector & A, const Vector & B)
{
 if (A.Get(0) == B.Get(0) && (A.Get(1)==B.Get(1) && A.Get(2)==B.Get(2))) return 1;
 else return 0;
}

inline int operator!=(const Vector & A, const Vector & B)
{
 if (A.Get(0)!=B.Get(0) || (A.Get(1)!=B.Get(1) || A.Get(2)!=B.Get(2))) return 1;
 else return 0;
}

inline std::ostream & operator<<(std::ostream & co, const Vector & A)
{
#ifdef VPRE
   co << std::setiosflags(std::ios::showpoint);
#endif
   co << std::setw(20) << std::setprecision(10) << A.Get(0);
   co << std::setw(20) << std::setprecision(10) << A.Get(1);
   co << std::setw(20) << std::setprecision(10) << A.Get(2);
   return co;
}

inline std::istream & operator>>(std::istream & ci, Vector &A)
{
 double x, y, z;
 ci >> x >> y >> z;
 A.SetX(x);
 A.SetY(y);
 A.SetZ(z);
 return ci;
}

inline double Dot(const Vector & A, const Vector & B)
{
 return (A.Get(0)*B.Get(0) + A.Get(1)*B.Get(1) + A.Get(2)*B.Get(2));
}

inline double Ang(const Vector & A, const Vector & B)
{
 double dot=Dot(A,B);
 double moda=A.Mod();
 double modb=B.Mod();
 return acos(dot/(moda*modb));
}

inline Vector Crux(const Vector & A, const Vector & B)
{
 Vector C;
 C.SetX(A.Get(1)*B.Get(2)-A.Get(2)*B.Get(1));
 C.SetY(A.Get(2)*B.Get(0)-A.Get(0)*B.Get(2));
 C.SetZ(A.Get(0)*B.Get(1)-A.Get(1)*B.Get(0));
 return C;
}

} // lpmd


#endif

