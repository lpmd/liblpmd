//
//
//

#ifndef __LPMD_ATOM_H__
#define __LPMD_ATOM_H__

#include "vector.h"
#include "paramlist.h"

namespace lpmd
{

//simbolos atomicos.
extern const std::string ElemSym[119];

//masas atomicas en uma.
extern const double ElemMass[119];

//radios atomicos covalente en Amstrong. 
extern const double ElemRad[119];

//
//
//
class AtomType: public ParamList
{
 public:
   std::string name;
  
   AtomType(const std::string & n);
};

//
//
//
class Atom
{
   public:
      // Constructores y Destructores
      Atom(): s(0), atomtype(NULL), charge(0.0e0) { }
      Atom(int S): s(S), atomtype(NULL), charge(0.0e0) { }
      Atom(int S, const Vector & P): s(S), atomtype(NULL), p(P), charge(0.0e0) { }
      Atom(int S, const Vector & P, const Vector & V): s(S), atomtype(NULL), p(P), v(V), charge(0.0e0) { }
      Atom(int S, const Vector & P, const Vector & V, const Vector & A): s(S), atomtype(NULL), p(P), v(V), a(A), charge(0.0e0) { }
      Atom(int S, const Vector & P, const Vector & V, const Vector & A, double c): s(S), atomtype(NULL), p(P), v(V), a(A), charge(c) { }

      //Settings and Gettings.
      void SetPos(const Vector & P) {p=P;}
      void SetVel(const Vector & V) {v=V;}
      void SetAccel(const Vector & A) {a=A;}
      void SetSpc(int S){s=S;}
      void SetCharge(double a) {charge = a;}

      const Vector & Position() const {return p;}
      const Vector & Velocity() const {return v;}
      const Vector & Acceleration() const {return a;}

      int Species() const { return s; }
      double Charge() const { return charge; }

      bool IsTypeSet() const { return (atomtype != NULL); }
      void SetType(AtomType & at) { atomtype = &at; }
      AtomType & Type() const { return (*atomtype); }

      const double & Mass() const { return ElemMass[s]; }
      std::string Symb() const { return ElemSym[s]; } 

      //Sobrecarga operador de asignacion.
      inline Atom& operator= (const Atom&A)
      {s=A.s;p=A.p;v=A.v;a=A.a;charge=A.charge;return(*this);}
   
      //Sobrecarga operadores de comparacion.
      friend int operator== (Atom&,Atom&);
      friend int operator!= (Atom&,Atom&);

      //Sobrecarga operadores salida
      friend std::ostream& operator<< (std::ostream&, const Atom&);
      friend std::istream& operator>> (std::istream&, Atom&);

   private:
      int s; //specie of atoms.
      AtomType * atomtype;
      Vector p, v, a; //position, velocity and aceleration of the atom
      double charge; //carga del atomo.
};

int ElemNum(std::string);

} // lpmd

#endif

