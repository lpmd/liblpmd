//
//
//

#ifndef __LPMD_ATOM_H__
#define __LPMD_ATOM_H__

#include <lpmd/color.h>
#include <lpmd/vector.h>
#include <lpmd/neighbor.h>
#include <lpmd/paramlist.h>
#include <iostream>
#include <cstdlib>


namespace lpmd
{

 extern const std::string ElemSym[119];

 extern const double ElemMass[119];

 extern const double ElemRad[119];

 inline int ElemNum(std::string el)
 {
  int r=0;
  for (int i=0;i<119;i++)
  {
   if (el == ElemSym[i]) 
   {
    r=i;
    break;
   }
  }
  return r;
 }

 const Vector dc(0,0.8,0.8);   // Default (blue) color for atoms

//
//
//
class AtomType: public ParamList
{
 public:
   std::string name;
  
   AtomType(const std::string & n);
};

class AtomNeighbors: public std::vector<Neighbor>
{
 public:
  void Add(Neighbor n) { push_back(n); }
  Neighbor & Get(int i) { return (*this)[i]; }
};

//
//
//
class Atom
{
  public:
    Atom();
    Atom(int S);
    Atom(int S, const Vector & P);
    Atom(int S, const Vector & P, const Vector & V);
    Atom(int S, const Vector & P, const Vector & V, const Vector & A);
    Atom(int S, const Vector & P, const Vector & V, const Vector & A, double c);

    void SetPos(const Vector & P);
    void SetVel(const Vector & V);
    void SetAccel(const Vector & A);
    void SetColor(const Vector & CLR);
    void SetColor(const double & x);
    void SetSpc(int S);
    void SetCharge(double a);
    void SetIndex(long a);

//Methods for atom neighbours.
    void AddNeighbor(Neighbor n)
    {
     if(atomneigh==NULL) atomneigh = new AtomNeighbors;
     atomneigh->Add(n);
    }
    void CleanNeighbors()
    {
     if(atomneigh!=NULL) delete atomneigh;
     atomneigh=NULL;
    }
    AtomNeighbors & Neighbors()
    {
     if(atomneigh==NULL) atomneigh = new AtomNeighbors;
     return (*atomneigh);
    }

    const Vector & Position() const;
    const Vector & Velocity() const;
    const Vector & Acceleration() const;
    const Vector & Color() const;
    
    int Species() const;
    int Index() const;
    double Charge() const;
    double ColorS() const;

    bool IsTypeSet() const;
    void SetType(AtomType & at);
    AtomType & Type() const;

    const double & Mass() const;
    std::string Symb() const;

    Atom & operator=(const Atom & A);
   
    friend int operator==(const Atom & a, const Atom & b);
    friend int operator!=(const Atom & a, const Atom & b);

    //FIXME : Comentadas por nuevo vector.h
    //friend std::ostream& operator<<(std::ostream & co, const Atom & a);
    //friend std::istream& operator>>(std::istream & ci, Atom & a);

    Atom *prev,*next; // for use in double linked lists

   private:
    int s;                         // Species of atom
    AtomType * atomtype;           // Atomtype
    AtomNeighbors * atomneigh;     // Neighbours of a atom.
    Vector p, v, a;                // Position, velocity and aceleration of the atom
    double charge;                 // charge of atom.
    long index;                    // index of atom, between 0 and N-1
    Vector clr;                    // Color of the atom in rgb format
};


//
//
//

inline Atom::Atom(): prev(NULL), next(NULL), s(0), atomtype(NULL), atomneigh(NULL), charge(0.0e0),  index(-1), clr(dc) { }

inline Atom::Atom(int S): prev(NULL), next(NULL), s(S), atomtype(NULL), atomneigh(NULL), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P):  prev(NULL), next(NULL),s(S), atomtype(NULL), atomneigh(NULL), p(P), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P, const Vector & V):  prev(NULL), next(NULL),s(S), atomtype(NULL), atomneigh(NULL), p(P), v(V), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P, const Vector & V, const Vector & A): prev(NULL), next(NULL),s(S), atomtype(NULL), atomneigh(NULL), p(P), v(V), a(A), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P, const Vector & V, const Vector & A, double c): prev(NULL), next(NULL),s(S), atomtype(NULL), atomneigh(NULL), p(P), v(V), a(A), charge(c), index (-1), clr(GetSpcColor(S)) { }

inline void Atom::SetPos(const Vector & P) { p=P; }

inline void Atom::SetVel(const Vector & V) { v=V; }

inline void Atom::SetAccel(const Vector & A) { a=A; }

inline void Atom::SetColor(const Vector & CLR) { clr=CLR;}

inline void Atom::SetColor(const double & x)
{
 double R,G,B,A=0.5;
 if      (0<=x && x<=A){ R=1.0-x/A; G=x/A; B=0; }
 else if (A<=x && x<=1.0){ R=0; G=(x-1)/(A-1); B=(x-A)/(1-A); }
 else {R=0; G=0.8; B=0.8;}
 Vector color(R,G,B);
 clr=color;
}

inline void Atom::SetSpc(int S) { s=S; clr=GetSpcColor(S);}

inline void Atom::SetCharge(double a) { charge = a; }

inline void Atom::SetIndex(long a) { index = a; }

inline const Vector & Atom::Position() const { return p; }

inline const Vector & Atom::Velocity() const { return v; }

inline const Vector & Atom::Acceleration() const { return a; }

inline const Vector & Atom::Color() const { return clr;}

inline double Atom::ColorS() const { return (clr[0]+clr[1]+clr[2])/3.0;}

inline int Atom::Species() const { return s; }

inline int Atom::Index() const { return index; }

inline double Atom::Charge() const { return charge; }

inline bool Atom::IsTypeSet() const { return (atomtype != NULL); }

inline void Atom::SetType(AtomType & at) { atomtype = &at; }

inline AtomType & Atom::Type() const { return (*atomtype); }

inline const double & Atom::Mass() const { return ElemMass[s]; }

inline std::string Atom::Symb() const { return ElemSym[s]; } 

inline Atom& Atom::operator=(const Atom & A) 
{
 s = A.s;
 p = A.p;
 v = A.v;
 a = A.a;
 charge = A.charge;
 index = A.index;
 clr=A.clr;
 return(*this);
}

inline int operator==(const Atom & A, const Atom & B)
{
 return ((A.Species() == B.Species()) && (A.Position()-B.Position()).Module() < 1.0E-10);
}

inline int operator!=(const Atom & A, const Atom & B) { return !(operator==(A, B)); }

/* FIXME: commented
inline std::ostream & operator<<(std::ostream & co, const lpmd::Atom & A)
{
///FIXME : El atomo debe mostrar su propiedad atomtype, esto es usado en WriteAll de particles, para dumping.
 co << A.Symb() << " " << A.Position() << " " << A.Velocity() << " " << A.Acceleration();
 return co;
}
*/

/* FIXME: commented
inline std::istream& operator>>(std::istream & ci, Atom & A)
{
 Vector pos, vel, ace;
 std::string symb;
 ci >> symb >> pos >> vel >> ace;
 A.SetSpc(ElemNum(symb));
 A.SetPos(pos);
 A.SetVel(vel);
 A.SetAccel(ace);
 return ci;
}
*/

//
//
//

} // lpmd

#endif

