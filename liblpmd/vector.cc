//
//
//

#include <lpmd/vector.h>
#include <vector>
#include <lpmd/util.h>

lpmd::Vector::Vector(std::string s)
{
 if (s.size()<7) {std::cerr << "Warning SET vector from string less as 7" << '\n';}
 s.erase(0,1);
 s.erase(s.size()-1,1);
 std::vector<std::string> v = SplitTextLine(s,',');
 if(v.size()!=3) { std::cerr << "Warning SET vector from string diferent of 3 numbers!!! " << '\n';}
 SetX(atof(v[0].c_str()));
 SetY(atof(v[1].c_str()));
 SetZ(atof(v[2].c_str()));
}
//Sobrecarga operadores de comparacion.
int lpmd::operator==(const lpmd::Vector & A, const lpmd::Vector & B)
{
 std::cerr << "DEBUG Vector::operator==" << '\n';
 if (A.Get(0) == B.Get(0) && (A.Get(1)==B.Get(1) && A.Get(2)==B.Get(2))) return 1;
 else return 0;
}
int lpmd::operator!=(const lpmd::Vector & A, const lpmd::Vector & B)
{
 std::cerr << "DEBUG Vector::operator!=" << '\n';
 if (A.Get(0)!=B.Get(0) || (A.Get(1)!=B.Get(1) || A.Get(2)!=B.Get(2))) return 1;
 else return 0;
}

//Sobrecarga operadores in/out.
std::ostream& lpmd::operator<<(std::ostream& co, const lpmd::Vector &A)
{
#ifdef VPRE
   co<<std::setiosflags(std::ios::showpoint);
#endif
   co << std::setw(20) << std::setprecision(10) << A.Get(0);
   co << std::setw(20) << std::setprecision(10) << A.Get(1);
   co << std::setw(20) << std::setprecision(10) << A.Get(2);
   return co;
}
std::istream& lpmd::operator>>(std::istream& ci, lpmd::Vector &A)
{
 double x,y,z;
 ci>>x>>y>>z;
 A.SetX(x);
 A.SetY(y);
 A.SetZ(z);
 return ci;
}

//Friends.

double lpmd::Dot(const Vector& A, const Vector& B)
{
 return (A.Get(0)*B.Get(0) + A.Get(1)*B.Get(1) + A.Get(2)*B.Get(2));
}

double lpmd::Ang(const Vector& A, const Vector& B)
{
 double dot=Dot(A,B);
 double moda=A.Mod();
 double modb=B.Mod();
 return acos(dot/(moda*modb));
}

lpmd::Vector lpmd::Crux(const Vector& A, const Vector& B)
{
 lpmd::Vector C(0,0,0);
 C.SetX(A.Get(1)*B.Get(2)-A.Get(2)*B.Get(1));
 C.SetY(A.Get(2)*B.Get(0)-A.Get(0)*B.Get(2));
 C.SetZ(A.Get(0)*B.Get(1)-A.Get(1)*B.Get(0));
 return C;
}

//Algunas Funciones.
void lpmd::Vector::Norm()
{
 double m=Mod();
 q[0]=q[0]/m;
 q[1]=q[1]/m;
 q[2]=q[2]/m;
}

