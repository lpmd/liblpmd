//
//
//

#ifndef __LPMD_ATOM_H__
#define __LPMD_ATOM_H__

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
	 Vector GetSpcColor(int spc);
    
    int Species() const;
    int Index() const;
    double Charge() const;

    bool IsTypeSet() const;
    void SetType(AtomType & at);
    AtomType & Type() const;

    const double & Mass() const;
    std::string Symb() const;

    Atom & operator=(const Atom & A);
   
    friend int operator==(const Atom & a, const Atom & b);
    friend int operator!=(const Atom & a, const Atom & b);

    friend std::ostream& operator<<(std::ostream & co, const Atom & a);
    friend std::istream& operator>>(std::istream & ci, Atom & a);

    Atom *prev,*next; // for use in double linked lists

   private:
    int s;                         // Species of atom
    AtomType * atomtype;           // Atomtype
    AtomNeighbors * atomneigh;    // Neighbours of a atom.
    Vector p, v, a;                // Position, velocity and aceleration of the atom
    double charge;                 // charge of atom.
    long index;                    // index of atom, between 0 and N-1
    Vector clr;						  // Color of the atom in rgb format
};


//
//
//
const Vector DefCol(0,0.8,0.8);
inline Vector Atom::GetSpcColor(int spc)
{
	Vector ans;
	if 		(spc==1) ans=(e1+e2+e3);						//Hydrogen (colorless)
	else if 	(spc==2) ans=(e1+e2+e3);						//Helium (colorless)
	else if 	(spc==3) ans=0.75*(e1+e2+e3);					//Lithium (silvery)
	else if 	(spc==4) ans=0.35*(e1+e2+e3);					//Beryllium (gray)
	else if 	(spc==5) ans=(139*e1+69*e2+19*e3)/255.0;	//Boron (brownish)
	else if 	(spc==6) ans=0.1*(e1+e2+e3);					//Carbon (black)
	else if 	(spc==7) ans=(e1+e2+e3);						//Nitrogen (colorless)
	else if 	(spc==8) ans=(e1+e2+e3);						//Oxigen (colorless)
	else if 	(spc==9) ans=0.8*e2;								//Fluorine (Greenish)
	else if 	(spc==10)ans=(e1+e2+e3);						//Neon (colorless)
	else if 	(spc==11)ans=0.75*(e1+e2+e3);					//Sodium (silvery)
	else if 	(spc==12)ans=0.35*(e1+e2+e3);					//Magnesium (grayish)
	else if 	(spc==13)ans=0.75*(e1+e2+e3);					//Aluminum  (silver)
	else if 	(spc==14)ans=0.35*(e1+e2+e3);					//Silicon   (gray)
	else if 	(spc==15)ans=(e1+e2+e3);						//Phosphorus (white)
	else if 	(spc==16)ans=(e1+e2);							//Sulfur (yellow)
	else if 	(spc==17)ans=(e2);								//Chlorine (green)
	else if 	(spc==18)ans=(e1+e2+e3);						//Argon  (colorless)
	else if 	(spc==19)ans=0.75*(e1+e2+e3);					//Potassium (silvery)
	else if 	(spc==20)ans=0.75*(e1+e2+e3);					//Calcium (silvery)
	else if 	(spc==21)ans=0.75*(e1+e2+e3);					//Scandium (silvery)
	else if 	(spc==22)ans=0.75*(e1+e2+e3);					//Titanium (silverish)
	else if 	(spc==23)ans=0.75*(e1+e2+e3);					//Vanadium (silverish)
	else if 	(spc==24)ans=0.35*(e1+e2+e3);					//Chromium (gray)
	else if 	(spc==25)ans=0.75*(e1+e2+e3);					//Manganese (silverish)
	else if 	(spc==26)ans=0.75*(e1+e2+e3);					//Iron (silvery)
	else if 	(spc==27)ans=0.75*(e1+e2+e3);					//Cobalt (silvery)
	else if 	(spc==28)ans=(e1+e2+e3);						//Nickel (white)
	else if 	(spc==29)ans=(255*e1+69*e2)/255.0;			//Copper (red/orange)
	else if 	(spc==30)ans=(e3);								//Zinc (bluish)
	else if 	(spc==31)ans=211*(e1+e2+e3)/255.0;			//Gallium (White/Silver)
	else if 	(spc==32)ans=0.35*(e1+e2+e3);					//Germanium (grayish)
	else if 	(spc==33)ans=0.5*(e1+e2+e3);					//Arsenic (Gray)
	else if 	(spc==34)ans=0.5*(e1+e2+e3);					//Selenium (Gray)
	else if 	(spc==35)ans=(e1);								//Bromine (Red)
	else if 	(spc==36)ans=(e1+e2+e3);						//Krypton (colorless)
	else if 	(spc==37)ans=0.75*(e1+e2+e3);					//Rubidium (silver)
	else if 	(spc==38)ans=(e1+e2);							//Strontium (yellowish)
	else if 	(spc==39)ans=0.75*(e1+e2+e3);					//Yttrium (silvery)
	else if 	(spc==40)ans=0.35*(e1+e2+e3);					//Zirconium (Grayish)
	else if 	(spc==41)ans=(e1+e2+e3);						//Niobium (white)
	else if 	(spc==42)ans=0.75*(e1+e2+e3);					//Molybdenum (silverish)
	else if 	(spc==43)ans=DefCol;								//Technetium (unknown)
	else if 	(spc==44)ans=0.75*(e1+e2+e3);					//Ruthenium (silvery)
	else if 	(spc==45)ans=0.75*(e1+e2+e3);					//Rhodium (silverish)
	else if 	(spc==46)ans=(e1+e2+e3);						//Palladium (white)
	else if 	(spc==47)ans=0.75*(e1+e2+e3);					//Silver (silver)
	else if 	(spc==48)ans=0.75*(e1+e2+e3);					//Cadmium (silver)
	else if 	(spc==49)ans=0.75*(e1+e2+e3);					//Indium (silver)
	else if 	(spc==50)ans=(e1+e2+e3);						//Tin (white)
	else if 	(spc==51)ans=e3;									//Antimony (bluish)
	else if 	(spc==52)ans=0.75*(e1+e2+e3);					//Tellurium (silverish)
	else if 	(spc==53)ans=0.1*(e1+e2+e3);					//Iodine (blackish)
	else if 	(spc==54)ans=(e1+e2+e3);						//Xenon  (colorless)
	else if 	(spc==55)ans=0.75*(e1+e2+e3);					//Cesium  (silver)
	else if 	(spc==56)ans=0.75*(e1+e2+e3);					//Barium  (silver)
	else if 	(spc==72)ans=0.75*(e1+e2+e3);					//Hafnium  (silver)
	else if 	(spc==73)ans=0.5*(e1+e2+e3);					//Tantalum  (gray)
	else if 	(spc==74)ans=0.75*(e1+e2+e3);					//Tungsten (silver)
	else if 	(spc==75)ans=0.75*(e1+e2+e3);					//Rhenium (silver)
	else if 	(spc==76)ans=0.75*(e1+e2+e3);					//Osmium (silver)
	else if 	(spc==77)ans=(e1+e2+e3);						//Iridium (white)
	else if 	(spc==78)ans=0.75*(e1+e2+e3);					//Platinum (silverish)
	else if 	(spc==79)ans=(255*e1+215*e2);					//Gold (gold)
	else if 	(spc==80)ans=0.75*(e1+e2+e3);					//Mercury (silver)
	else if 	(spc==81)ans=e3;									//Thallium (bluish)
	else if 	(spc==82)ans=e3;									//Lead (bluish)
	else if 	(spc==83)ans=e1+e2+e3;							//Bismuth (white)
	else if 	(spc==84)ans=DefCol;								//Polonium (unknown)
	else if 	(spc==85)ans=DefCol;								//Astatine (unknown)
	else if 	(spc==86)ans=e1+e2+e3;							//Radon (colorless)
	else if 	(spc==87)ans=DefCol;								//Francium (unknown)
	else if 	(spc==88)ans=0.75*(e1+e2+e3);					//Radium (silverish)
	else if 	(spc==104)ans=DefCol;							//Rutherfordium (unknown)
	else if 	(spc==105)ans=DefCol;							//Dubnium (unknown)
	else if 	(spc==106)ans=DefCol;							//Seaborgium (unknown)
	else if 	(spc==107)ans=DefCol;							//Bohrium (unknown)
	else if 	(spc==108)ans=DefCol;							//Hassium (unknown)
	else if 	(spc==109)ans=DefCol;							//Meitnerium (unknown)
	else if 	(spc==110)ans=DefCol;							//Ununnilium (unknown)
	else if 	(spc==111)ans=DefCol;							//Unununium (unknown)
	else if 	(spc==112)ans=DefCol;							//Ununbium (unknown)
	else					ans=DefCol;
	return ans;
}


inline Atom::Atom(): prev(NULL), next(NULL), s(0), atomtype(NULL), charge(0.0e0),  index(-1), clr(DefCol) { }

inline Atom::Atom(int S): prev(NULL), next(NULL), s(S), atomtype(NULL), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P):  prev(NULL), next(NULL),s(S), atomtype(NULL), p(P), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P, const Vector & V):  prev(NULL), next(NULL),s(S), atomtype(NULL), p(P), v(V), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P, const Vector & V, const Vector & A): prev(NULL), next(NULL),s(S), atomtype(NULL), p(P), v(V), a(A), charge(0.0e0), index(-1), clr(GetSpcColor(S)) { }

inline Atom::Atom(int S, const Vector & P, const Vector & V, const Vector & A, double c): prev(NULL), next(NULL),s(S), atomtype(NULL), p(P), v(V), a(A), charge(c), index (-1), clr(GetSpcColor(S)) { }

inline void Atom::SetPos(const Vector & P) { p=P; }

inline void Atom::SetVel(const Vector & V) { v=V; }

inline void Atom::SetAccel(const Vector & A) { a=A; }

inline void Atom::SetColor(const Vector & CLR) { clr=CLR;}

inline void Atom::SetSpc(int S) { s=S; clr=GetSpcColor(S);}

inline void Atom::SetCharge(double a) { charge = a; }

inline void Atom::SetIndex(long a) { index = a; }

inline const Vector & Atom::Position() const { return p; }

inline const Vector & Atom::Velocity() const { return v; }

inline const Vector & Atom::Acceleration() const { return a; }

inline const Vector & Atom::Color() const { return clr;}

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
 if ((A.Species() == B.Species()) && (A.Position() == B.Position())) return 1;
 else return 0;
}

inline int operator!=(const Atom & A, const Atom & B)
{
 if ((A.Species() != B.Species()) || (A.Position() != B.Position())) return 1;
 else return 0;
}

inline std::ostream & operator<<(std::ostream & co, const lpmd::Atom & A)
{
 //FIXME : El atomo debe mostrar su propiedad atomtype, esto es usado en WriteAll de particles, para dumping.
 co << A.Symb() << " " << A.Position() << " " << A.Velocity() << " " << A.Acceleration();
 return co;
}

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

//
//
//

} // lpmd

#endif

