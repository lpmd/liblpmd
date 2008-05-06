//
//
//


#include <lpmd/particles.h>
#include <lpmd/util.h>
#include <lpmd/physunits.h>

using namespace lpmd;

Particles::Particles(): spec(NULL) { }

Particles::~Particles() { if (spec != NULL) delete [] spec; }

std::ostream & operator << (std::ostream & os,const Particles &I)
{
 os << I.Size() << std::endl <<std::endl;
 for (long i=0;i<I.Size();i++) {os << I[i] <<std::endl;}
 return os;
}

void Particles::AssignIndex()
{
 for(int i=0;i<Size();++i)
 {
  Atom a = GetAtom(i);
  a.SetIndex(i);
  SetAtom(a,i);
 }
}

void Particles::InitVelocities()
{
 Vector vel, totalp;
 long nparts = Size();
 for (int i=0;i<nparts;++i)
 {
  vel = Vector(2.0*drand48()-1.0, 2.0*drand48()-1.0, 2.0*drand48()-1.0);
  totalp = totalp + vel*(GetAtom(i).Mass());
  Atom a = GetAtom(i);
  a.SetVel(vel);
  SetAtom(a, i);
 }
 totalp /= nparts;
 for (int i=0;i<nparts;++i)
 {
  vel = GetAtom(i).Velocity();
  vel = vel - totalp/GetAtom(i).Mass();
  Atom a = GetAtom(i);
  a.SetVel(vel);
  SetAtom(a, i);
 }
}

void Particles::SetTemperature(double temp, double dt, double tau)
{
 Vector vel;
 double xi, ti = Temperature();
 for (int i=0;i<Size();++i) 
 {
  vel = GetAtom(i).Velocity();
  xi = sqrt(1.0 + (double(dt)/tau)*(temp/ti - 1.0));
  vel.Scale(xi);
  Atom a = GetAtom(i);
  a.SetVel(vel);
  SetAtom(a, i);
 }
}

double Particles::KineticEnergy() const
{
 double K = 0.0;
 Vector vel;
 for (long i=0;i<Size();++i)
 {
  vel = GetAtom(i).Velocity();
  K += 0.5*GetAtom(i).Mass()*vel.Mod2();
 }
 return K*KIN2EV;
}

double Particles::Temperature() const
{
 return (2.0/3.0)*KineticEnergy()/(KBOLTZMANN*double(Size()));
}

double Particles::Momentum() const
{
 Vector vel, totalp;
 for (int i=0;i<Size();++i)
 {
  vel = GetAtom(i).Velocity();
  totalp = totalp + vel*GetAtom(i).Mass();
 }
 return totalp.Mod();
}

void Particles::NumEspec() 
{
 int tmp = 1;
 if (spec != NULL) delete [] spec;
 spec = new int[1];
 spec[0] = GetAtom(0).Species();
 int * sptmp = new int[1];
 sptmp[0] = spec[0];
 bool Q=false;
 for(int i=1;i<Size();i++)
 {
  int tmp2=0;
  for(int j=0;j<tmp;j++)
  {
   if(GetAtom(i).Species()!=spec[j]) {tmp2++;}
  }
  if(tmp2==0) {Q=false;}
  else if(tmp2<tmp) {Q=false;}
  else if(tmp2==tmp) {Q=true;}
  if(Q==1)
  {
   tmp++;
   if (spec != NULL) delete [] spec;
   spec=new int[tmp];
   for(int k=0;k<tmp-1;k++) spec[k]=sptmp[k];
   spec[tmp-1]=GetAtom(i).Species();
   if (sptmp != NULL) delete [] sptmp;
   sptmp=new int[tmp];
   for(int k=0;k<tmp;k++) sptmp[k]=spec[k];
  }
  Q=false;
 }
 delete [] sptmp;
 nspec = tmp;
}

void Particles::WriteAll(std::ostream &out)
{
 out << A.size() <<std::endl;
 for(unsigned int i=0;i<A.size();i++)
 {
  out << A[i] <<std::endl;
 }
}

std::list<std::string> Particles::SpeciesTriplets() const
{
 std::list<std::string> tmp1;
 for (int i=0;i<nspec;++i)
 {
  for (int j=0;j<nspec;++j)
  {
   for (int k=0;k<nspec;++k) tmp1.push_back(ElemSym[spec[i]]+"-"+ElemSym[spec[j]]+"-"+ElemSym[spec[k]]);
  }
 }
 return tmp1;
}

std::list<std::string> Particles::SpeciesPairs() const
{
 std::list<std::string> tmp1;
 for (int i=0;i<nspec;++i)
  for (int j=i;j<nspec;++j) tmp1.push_back(ElemSym[spec[i]]+"-"+ElemSym[spec[j]]);
 return tmp1;
}

std::list<std::string> Particles::RepeatedSpeciesPairs() const
{
 std::list<std::string> tmp1, tmp2;
 for (int i=0;i<nspec;++i)
  for (int j=0;j<nspec;++j)
  {
   tmp2.clear();
   tmp2.push_back(ElemSym[spec[i]]);
   tmp2.push_back(ElemSym[spec[j]]);
   std::string s1 = tmp2.front();
   tmp2.pop_front();
   std::string s2 = tmp2.front();
   tmp2.pop_front();
   tmp1.push_back(s1+"-"+s2);
  }
 return tmp1;
}

std::list<std::string> Particles::SpeciesList() const
{
 std::list<std::string> tmp1;
 tmp1.clear();
 for(int i=0;i<nspec;i++)
 {
  tmp1.push_back(ElemSym[spec[i]]);
 }
 tmp1.unique();
 return tmp1;
}

std::vector<int> Particles::NSpecies() const
{
 std::vector<int> tmp;
 std::list<std::string> especies=SpeciesList();
 for(std::list<std::string>::const_iterator it=especies.begin();it!=especies.end();++it)
 {
  int counter=0;
  std::string symbol=*it;
  for(int i=0;i<Size();++i)
  {
   if(GetAtom(i).Symb()==symbol) counter++;
  }
  tmp.push_back(counter);
 }
 return tmp;
}


/*
std::istream & operator >> (std::istream & in, Particles &I)
{
   char line[255];
   int L;
   in >> L;
   char a;
   in.get(a);
   in.get(a);
   if(a=='\n')
   {}
   else
   {
      in.getline(line,sizeof(line));
   }
   I.setN(L);
   Atom tmp;
   for(int i=0;i<L;i++) {in >> tmp;I.setpar(tmp,i);}
   return in;
}*/


