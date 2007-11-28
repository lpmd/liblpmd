//
//
//

#include <lpmd/atom.h>

using namespace lpmd;

const std::string lpmd::ElemSym[119]=
{
 "e","H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl",
 "Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br",
 "Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I",
 "Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At",
 "Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Uun","Uuu","Uub","Uut","Uuq","Uup","Uuh","Uus","Uuo"
};

const double lpmd::ElemMass[119]=
{  
   5.4859e-4,1.00794,4.002602,6.941,9.01218,10.811,12.011,14.00674,15.9994,18.9984303,
   20.1797,22.989768,24.305,26.981539,28.0855,30.973762,32.066,35.4527,
   39.948,39.0983,40.078,44.95591,47.88,50.9415,51.9961,54.93805,55.847,58.9332,58.6934,63.546,65.39,69.723,72.61,74.92159,78.96,79.904,
   83.8,85.4678,87.62,88.90585,91.224,92.90638,95.94,97.9072,102.07,102.9055,106.42,107.8682,112.411,114.818,118.71,121.760,127.6,126.90447,
   131.29,132.90543,137.327,138.9055,140.115,140.90765,144.24,144.9127,150.36,151.965,157.25,158.92534,162.50,164.93032,167.26,168.93421,173.04,174.967,178.49,180.9479,183.84,186.207,190.23,192.22,195.08,196.96654,200.59,204.3833,207.2,208.98037,208.9824,209.9871,
   222.0176,223.0197,226.0254,227.0278,232.0381,231.03588,238.0289,237.048,244.0642,243.0614,247.0703,247.0703,251.0796,252.083,257.0951,258.1,259.1009,262.11,261,262,266,264,269,268,269,272,277,0,289,0,0,0,0
};

const double lpmd::ElemRad[119]=
{
   0.0e0,0.32,0.0e0,1.63,0.9,0.81,0.77,0.75,0.74,0.72,
   0.71,1.54,1.36,1.18,1.18,1.10,1.04,0.99,
   0.98,2.03,1.74,1.44,1.32,1.22,1.18,1.17,1.17,1.16,1.15,1.17,1.25,1.25,1.22,1.21,1.17,1.14,
   1.12,2.16,1.91,1.62,1.45,1.34,1.30,1.27,1.25,1.25,1.28,1.34,1.48,1.44,1.40,1.41,1.37,1.33,
   1.31,2.35,1.98,1.69,1.65,1.65,1.64,1.63,1.66,1.85,1.61,1.59,1.59,1.58,1.57,1.56,1.70,1.56,1.44,1.34,1.30,1.28,1.26,1.26,1.29,1.34,1.44,1.48,1.46,1.46,1.46,1.45,
   0.0e0,0.0,0.0,0.0,1.65,0.0,1.42,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0
};

int operator==(Atom& A, Atom& B)
{
 if ((A.Species() == B.Species()) && (A.Position() == B.Position())) return 1;
 else return 0;
}

int operator!=(Atom& A, Atom& B)
{
 if ((A.Species() != B.Species()) || (A.Position() != B.Position())) return 1;
 else return 0;
}

std::ostream& lpmd::operator<<(std::ostream& co, const lpmd::Atom& A)
{
 //FIXME : El atomo debe mostrar su propiedad atomtype, esto es usado en WriteAll de particles, para dumping.
 co << A.Symb() << " " << A.Position() << " " << A.Velocity() << " " << A.Acceleration();
 return co;
}

std::istream& lpmd::operator>>(std::istream& ci, Atom& A)
{
 Vector pos,vel,ace;
 std::string symb;
 ci >> symb >> pos >> vel >> ace;
 A.SetSpc(ElemNum(symb));
 A.SetPos(pos);
 A.SetVel(vel);
 A.SetAccel(ace);
 return ci;
}

int lpmd::ElemNum(std::string el)
{
   int r=0;
   for(int i=0;i<119;i++){if(el==ElemSym[i]){r=i;break;}}
   return r;
}

//
//
//
AtomType::AtomType(const std::string & n): name(n)
{
 // Valores por defecto para el AtomType
 AssignParameter("fixedpos", "false");
 AssignParameter("fixedvel", "false");
 AssignParameter("fixedacc", "false");
}


