//
//
//

#include <lpmd/session.h>
#include <lpmd/particleset.h>

using namespace lpmd;

ParticleSet::ParticleSet() { spec = NULL; }

ParticleSet::ParticleSet(const ParticleSet & ps)
{
 spec = NULL;
 for (unsigned long int i=0;i<ps.size();++i) Create(new Atom(ps[i]));
 NumEspec();
 AssignIndex();
}

ParticleSet::~ParticleSet()
{
 for (unsigned long int i=0;i<managed.size();++i) delete managed[i];
 delete [] spec;
}

Atom & ParticleSet::Create(Atom * at)
{
 push_back(at);
 managed.push_back(at);
 return (*at);
}

void ParticleSet::Destroy(Atom * at)
{
 for (std::vector<Atom *>::iterator it=managed.begin();it!=managed.end();++it)
 {
  if ((*it) == at) 
  {
   delete at;
   managed.erase(it);
   break;
  }
 }
}

ParticleSet & ParticleSet::operator=(const ParticleSet & ps)
{
 if (&ps != this)
 {
  clear();
  for (unsigned long int i=0;i<managed.size();++i) delete managed[i];
  managed.clear();
  for (unsigned long int i=0;i<ps.size();++i) Create(new Atom(ps[i]));
 }
 NumEspec();
 AssignIndex();
 return (*this);
}

void ParticleSet::NumEspec()
{
 if (size() == 0) return;   // Si no hay particulas no hace nada, esto evita bug al intentar leer el atomo 0
 int tmp = 1;
 if (spec != NULL) delete [] spec;
 spec = new int[1];
 spec[0] = operator[](0).Species();
 int * sptmp = new int[1];
 sptmp[0] = spec[0];
 bool Q=false;
 for(unsigned long int i=1;i<size();i++)
 {
  int tmp2=0;
  for(int j=0;j<tmp;j++)
  {
   if (operator[](i).Species()!=spec[j]) {tmp2++;}
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
   spec[tmp-1]=operator[](i).Species();
   if (sptmp != NULL) delete [] sptmp;
   sptmp=new int[tmp];
   for(int k=0;k<tmp;k++) sptmp[k]=spec[k];
  }
  Q=false;
 }
 delete [] sptmp;
 nspec = tmp;
 #warning "ELIMINAR ParticleSet::NumEspec() LO ANTES POSIBLE"
}

void ParticleSet::AssignIndex()
{
 #warning "ELIMINAR ParticleSet::AssignIndex() LO ANTES POSIBLE"
 for (unsigned long int i=0;i<size();++i) operator[](i).SetIndex(i);
}

std::list<std::string> ParticleSet::SpeciesList() const
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

std::list<std::string> ParticleSet::SpeciesTriplets() const
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

std::list<std::string> ParticleSet::SpeciesPairs() const
{
 std::list<std::string> tmp1;
 for (int i=0;i<nspec;++i)
  for (int j=i;j<nspec;++j) tmp1.push_back(ElemSym[spec[i]]+"-"+ElemSym[spec[j]]);
 return tmp1;
}

std::list<std::string> ParticleSet::RepeatedSpeciesPairs() const
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

std::vector<int> ParticleSet::NSpecies() const
{
 std::vector<int> tmp;
 std::list<std::string> especies=SpeciesList();
 for(std::list<std::string>::const_iterator it=especies.begin();it!=especies.end();++it)
 {
  int counter=0;
  std::string symbol=*it;
  for (unsigned long int i=0;i<size();++i)
  {
   if (operator[](i).Symb() == symbol) counter++;
  }
  tmp.push_back(counter);
 }
 return tmp;
}

void ParticleSet::WriteAll(std::ostream & out)
{
 out << size() << std::endl;
 for (unsigned long int i=0;i<size();i++) out << operator[](i) << std::endl;
}

void ParticleSet::InitVelocities()
{
 Vector vel, totalp;
 unsigned long int nparts = size();
 for (unsigned long int i=0;i<nparts;++i)
 {
  vel = Vector(2.0*drand48()-1.0, 2.0*drand48()-1.0, 2.0*drand48()-1.0);
  totalp = totalp + vel*operator[](i).Mass();
  operator[](i).SetVel(vel);
 }
 totalp /= nparts;
 for (unsigned long int i=0;i<nparts;++i)
 {
  vel = operator[](i).Velocity();
  vel = vel - totalp/operator[](i).Mass();
  operator[](i).SetVel(vel);
 }
}

void ParticleSet::SetTemperature(double temp, double dt, double tau)
{
 Vector vel;
 double xi, ti = Temperature();
 for (unsigned long int i=0;i<size();++i) 
 {
  vel = operator[](i).Velocity();
  xi = sqrt(1.0 + (double(dt)/tau)*(temp/ti - 1.0));
  vel.Scale(xi);
  operator[](i).SetVel(vel);
 }
}

double ParticleSet::KineticEnergy() const
{
 double K = 0.0;
 Vector vel;
 for (unsigned long int i=0;i<size();++i)
 {
  vel = operator[](i).Velocity();
  K += 0.5*operator[](i).Mass()*vel.Mod2();
 }
 return K*GlobalSession.GetDouble("kin2ev");
}

double ParticleSet::Temperature() const
{
 return (2.0/3.0)*KineticEnergy()/(GlobalSession.GetDouble("kboltzmann")*double(size()));
}

double ParticleSet::Momentum() const
{
 Vector vel, totalp;
 for (unsigned long int i=0;i<size();++i)
 {
  vel = operator[](i).Velocity();
  totalp = totalp + vel*operator[](i).Mass();
 }
 return totalp.Mod();
}


