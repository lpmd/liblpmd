/*
 *
 *
 *
 */

#include <lpmd/properties.h>
#include <lpmd/matrix.h>
#include <lpmd/configuration.h>
#include <lpmd/potential.h>
#include <lpmd/util.h>
#include <lpmd/configurationset.h>
#include <lpmd/storedconfiguration.h>

void lpmd::gdr(lpmd::Configuration & con, lpmd::Potential & pot, long int nb, double rcut, lpmd::Matrix & m)
{
 assert(&pot != 0); // icc 869
 lpmd::BasicParticleSet & atoms = con.Atoms();
 lpmd::BasicCell & cell = con.Cell();

 double dr = rcut/ double(nb);
 lpmd::Array<int> esp = atoms.Elements();
 int nsp = esp.Size();
 int N = atoms.Size();

 double **g, *gt;
 g = new double*[nb];
 for(int i=0;i<nb;i++) { g[i]=new double[(int)(nsp*(nsp+1)/2)]; }
 gt = new double[nb]; //total gdr
 for(int i=0;i<nb;i++) 
 { 
  gt[i]=0.0e0;
  for(int j=0;j<(int)(nsp*(nsp+1)/2);j++) g[i][j]=0.0e0;
 }
 int s=0;
 lpmd::Array<std::string> pairs;
 for(int i=0; i<esp.Size() ; ++i)
 {
  for(int j=i; j<esp.Size() ; ++j)
  {
   std::ostringstream ostr;
   ostr << ElemSym[esp[i]]<< "-" << ElemSym[esp[j]];
   pairs.Append(ostr.str());
  }
 }

 for(int i = 0 ; i < pairs.Size() ; ++i)	   
 {
  lpmd::Array<std::string> loa = lpmd::SplitSpeciesPair(pairs[i]); // lista de atomos
  int ne1=0,ne2=0;
  for(int a=0;a<N;a++)
  {
   if(atoms[a].Symbol()==loa[0]) ne1++;
   if(atoms[a].Symbol()==loa[1]) ne2++;				     }

   for(int j=0;j<N;++j)
   {
    if(atoms[j].Symbol()==loa[0])
    {
     lpmd::NeighborList & nlist = con.Neighbors(j,true,rcut);
     for(long int k=0; k<nlist.Size() ; ++k)
     {
      const lpmd::AtomPair & nn = nlist[k];
      if(nn.j->Symbol()==loa[1])
      {
       if(nn.r2<=rcut*rcut)
       {
	int ig=(long)floor(sqrt(nn.r2)/dr);
	g[ig][s]=g[ig][s]+(cell.Volume())/(4.0e0*M_PI*nn.r2*dr*ne1*ne2);
       }
      }
     }
    }
   }
   s++;
 }
 //Calcula el valor de g(r) total.
 int j=0;
 for(long int i=0; i< pairs.Size() ; ++i)
 {
  lpmd::Array<std::string> loa = lpmd::SplitSpeciesPair(pairs[i]); // lista de atomos
  int e1 = ElemNum(loa[0]);
  int e2 = ElemNum(loa[1]);
  int ne1=0,ne2=0;
  for(int a=0;a<N;a++)
  {
   if(atoms[a].Symbol()==loa[0]) ne1++;
   if(atoms[a].Symbol()==loa[1]) ne2++;
  }
  double ce1 = (double)ne1/(double)N;
  double ce2 = (double)ne2/(double)N;
  for(int k=0;k<nb;k++)
  {
   if(e1==e2) gt[k] = gt[k]+ce1*ce2*g[k][j];
   else {gt[k]=gt[k]+2*ce1*ce2*g[k][j];}
  }
  j++;
 }
 //
 // Output of g(r)
 //

 m = lpmd::Matrix(2 + nsp*(nsp+1)/2, nb);

 // Asigna los labels al objeto Matrix para cada columna
 m.SetLabel(0, "r");
 m.SetLabel(nsp*(nsp+1)/2+1, "total g(r)");
 j=1;
 for (long int i = 0 ; i < pairs.Size() ; ++i)
 {
  m.SetLabel(j, pairs[i]+" g(r)");
  j++;
 }
 // 
 for(int i=0;i<nb;i++)
 {
  m.Set(0, i, dr*i);
  for(j=0;j<(int)(nsp*(nsp+1)/2);j++)
  {
   m.Set(j+1, i, g[i][j]);
  }
  m.Set(nsp*(nsp+1)/2+1, i, gt[i]);
 }
 delete [] gt;
 for (int i=0;i<nb;i++) delete [] g[i];
 delete [] g;
}

void lpmd::vacf(lpmd::ConfigurationSet & hist, lpmd::Potential & pot, double dt, lpmd::Matrix & m)
{
 assert(&pot != 0); // icc 869
 int N = hist.Size(); 
 const Array <int> & species = hist[0].Atoms().Elements();
 int nsp = species.Size();
 int nat = hist[0].Atoms().Size();
 int level = int(Parameter(hist[0].GetTag(hist[0], "level")));

 double **vaf=new double*[(int)(N-1)/2];
 for(int i=0;i<(int)(N-1)/2;i++) {vaf[i]=new double[nsp];for(int j=0;j<nsp;j++) vaf[i][j]=0.0e0;}

 Vector ** velocities = new Vector*[N];
 for (int t=0;t<N;++t)  velocities[t] = new Vector[nat];

 if (level == 0)
 {
  //
  // Undo periodicity 
  //
  lpmd::StoredConfiguration scratch(hist[0]);
  lpmd::ParticleSet  scratch_atoms = scratch.Atoms();
  lpmd::BasicCell & cell = scratch.Cell();

  Vector ** noperiodic = new Vector*[N];
  for (int t=0;t<N;++t) noperiodic[t] = new Vector[nat];
  for (int i=0;i<nat;++i) noperiodic[0][i] = scratch_atoms[i].Position();

  for (int t=1;t<N;++t)
  {
   for (int i=0;i<nat;++i)
   {
    const Vector & v0 = scratch_atoms[0].Position() = hist[t-1].Atoms()[i].Position();
    const Vector & v1 = scratch_atoms[1].Position() = hist[t].Atoms()[i].Position();
    noperiodic[t][i] = noperiodic[t-1][i] + cell.Displacement(v0, v1);
   }
  }
  //
  //Evaluate and set velocities
  //
  for (int i=0;i<nat;++i) velocities[0][i]=(noperiodic[0][i]-noperiodic[1][i])/dt;

  for (int t=1;t<N-1;++t)
  {
   for (int i=0;i<nat;++i)
   {
    Vector vel = (noperiodic[t+1][i]-noperiodic[t][i])/dt;
    velocities[t][i] = vel;
   }
  }
 }
 if (level == 1)
 {
  for (int t=0;t<N;++t)
  {
   for (int i=0;i<nat;++i)
   {
    ParticleSet tmp = hist[t].Atoms();
    velocities[t][i] = tmp[i].Velocity();
   }
  }
 }

 int s=0;
 for(int e1=0;e1<nsp;e1++)	   
 {		 	
  int ne=0;
  for(int i=0;i<nat;i++) {if(hist[0].Atoms()[i].Z() == species[e1]) ne++;}
  for(int t0=0;t0<(int)(N-1)/2;t0++)
  {
   for(int t=0;t<(int)(N-1)/2;t++)
   {
    for(int i=0;i<nat;i++)
    {
     Vector v0n = velocities[t0][i];//simcell[t0].GetAtom(i).Velocity();
     Vector v1n = velocities[t0+t][i];//simcell[t0+t].GetAtom(i).Velocity();
     if(hist[t0].Atoms()[i].Z() == species[e1])
     {
      vaf[t][e1]+=Dot(v0n,v1n)/(ne*(int)(N-1)/2);
     }
    }
   }
  }
  s++;
 }

 for (int i=0;i<N;++i) delete[] velocities[i];
 delete [] velocities;

 //
 // Output of vacf
 //
 
 m = Matrix(nsp+1, (int)(N-1)/2);

 int k=1;
 for (int i=0;i<species.Size();++i)
 {
  m.SetLabel(k, ElemSym[species[i]]);
  k++;
 }
 m.SetLabel(0,"time");

 for(int i=0;i<(int)(N-1)/2;++i)
 {
  m.Set(0, i, dt*i);
  for (int j=0;j<nsp;++j)
  {
   m.Set(j+1, i, vaf[i][j]);
  }
 }
 for(int i=0;i<(int)(N-1)/2;++i) delete [] vaf[i];
 delete [] vaf;
}

