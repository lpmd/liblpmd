//
//
//

#ifndef __LPMD_VECTOR_H__
#define __LPMD_VECTOR_H__

#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace lpmd
{

class Vector
{
 public:
   //Constructores y destructores.
   Vector() {q[0]=0.0e0;q[1]=0.0e0;q[2]=0.0e0;};
   Vector(double val) {q[0]=val;q[1]=val;q[2]=val;};
   Vector(double x, double y, double z){q[0]=x;q[1]=y;q[2]=z;};
   Vector(const Vector & c){q[0]=c.q[0];q[1]=c.q[1];q[2]=c.q[2];};
   Vector(std::string s);
   ~Vector(){};
 
   //Settings & Gettings.
   void SetX(double x) {q[0]=x;};
   void SetY(double y) {q[1]=y;};
   void SetZ(double z) {q[2]=z;};

   void Set(int i, double v) {q[i]=v;};
   void Set(double x, double y, double z) {q[0]=x;q[1]=y;q[2]=z;};

   const double & Get(int i) const {return q[i];};
   const double & GetX() const {return q[0];};
   const double & GetY() const {return q[1];};
   const double & GetZ() const {return q[2];};

   //Algunas Funciones.
   double Mod() const {return sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]);};
   double Mod2() const {return q[0]*q[0]+q[1]*q[1]+q[2]*q[2];};
   void Scale(double f) { for (int i=0;i<3;++i) q[i] = q[i]*f; };
   void Norm();
   void Zero() {q[0]=0.0e0;q[1]=0.0e0;q[2]=0.0e0;};
   std::string GetPOV() const
   {std::ostringstream ostr;ostr<<"<"<<q[0]<<" ,"<<q[1]<<" ,"<<q[2]<<">";return ostr.str();}

   //Sobrecarga operadores aritmeticos.
   inline friend Vector operator+ (const Vector&A,const Vector&B)
     {return Vector(A.q[0]+B.q[0],A.q[1]+B.q[1],A.q[2]+B.q[2]);};
   inline friend Vector operator+ (const Vector&A,const double&a)
     {return Vector(A.q[0]+a,A.q[1]+a,A.q[2]+a);};
   inline friend Vector operator- (const Vector&A,const Vector&B)
     {return Vector(A.q[0]-B.q[0],A.q[1]-B.q[1],A.q[2]-B.q[2]);};
   inline friend Vector operator- (const Vector&A,const double&a)
     {return Vector(A.q[0]-a,A.q[1]-a,A.q[2]-a);};
   inline friend Vector operator* (const Vector&A,const Vector&B)
     {return Vector(A.q[0]*B.q[0],A.q[1]*B.q[1],A.q[2]*B.q[2]);};
   inline friend Vector operator* (const Vector&A,const double&a)
     {return Vector(A.q[0]*a,A.q[1]*a,A.q[2]*a);};
   inline friend Vector operator/ (const Vector&A,const double&a)
     {return Vector(A.q[0]/a,A.q[1]/a,A.q[2]/a);};
   inline friend Vector operator/ (const Vector&A,const Vector&B)
     {return Vector(A.q[0]/B.q[0],A.q[1]/B.q[1],A.q[2]/B.q[2]);};

   // 
   inline const Vector & operator*=(const double a)
   {
    for (int j=0;j<3;++j) q[j] *= a; 
    return (*this); 
   }
   inline const Vector & operator/=(const double a)
   {
    for (int j=0;j<3;++j) q[j] /= a;
    return (*this); 
   }

   //Sobrecarga operador de asignacion.
   inline Vector& operator=(const Vector & A) { q[0]=A.q[0];q[1]=A.q[1];q[2]=A.q[2];return(*this);};

private:
   double q[3];
};

//Sobrecarga operadores de comparacion.
int operator==(const Vector &, const Vector &);
int operator!=(const Vector &, const Vector &);

//Sobrecarga operadores in/out.
std::ostream & operator<<(std::ostream &, const lpmd::Vector &);
std::istream & operator>>(std::istream &, lpmd::Vector &);

//Friends.
double Dot(const Vector&, const Vector&);
double Ang(const Vector&, const Vector&);
Vector Crux(const Vector&, const Vector&);

} // lpmd


#endif

