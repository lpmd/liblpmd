#include <lpmd/properties.h>
#include <lpmd/matrix.h>
#include <lpmd/configuration.h>
#include <lpmd/potential.h>
#include <lpmd/util.h>

lpmd::Matrix* gdr(lpmd::Configuration & con, lpmd::Potential & pot, long int nb, double rcut)
{
 lpmd::BasicParticleSet & atoms = con.Atoms();
 lpmd::BasicCell & cell = con.Cell();
 assert(atoms.Size()==0);

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
 //FIXME : Construye speciesPairs aquí, ¿no es metodo de la API? ¿o si?.
 //const std::list<std::string> lst = simcell.SpeciesPairs();
 lpmd::Array<std::string> pairs;
 for(int i=0; i<esp.Size() ; ++i)
 {
  for(int j=i; j<esp.Size() ; ++j)
  {
   std::ostringstream ostr;
   ostr << esp[i] << "-" << esp[j];
   pairs.Append(ostr.str());
  }
 }

 for(int i = 0 ; i < pairs.Size() ; ++i)	   
 {
  //Hace funcional cada una de las especies de los pares.
  lpmd::Array<std::string> loa = lpmd::SplitSpeciesPair(pairs[i]); // lista de atomos
  //Cuenta los atomos de cada especie atomica.
  int ne1=0,ne2=0;
  for(int m=0;m<N;m++)
  {
   if(atoms[m].Symbol()==loa[0]) ne1++;
   if(atoms[m].Symbol()==loa[1]) ne2++;				     }

   //Comienza la iteracion principal para el calculo de g(r).
   for(int i=0;i<N;++i)
   {
    if(atoms[i].Symbol()==loa[0])
    {
     lpmd::NeighborList & nlist = con.Neighbors(i,true,rcut);
     for(long int k=0; k<nlist.Size() ; ++k)
     {
      const lpmd::AtomPair & nn = nlist[k];
      if(nn.j->Symbol()==loa[1])
      {
       if(nn.r*nn.r<=rcut*rcut)
       {
	int ig=(long)floor(nn.r/dr);
	g[ig][s]=g[ig][s]+(cell.Volume())/(4.0e0*M_PI*nn.r*nn.r*dr*ne1*ne2);
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
  //Hace funcional cada una de las especies de los pares.
  lpmd::Array<std::string> loa = lpmd::SplitSpeciesPair(pairs[i]); // lista de atomos
  int e1 = ElemNum(loa[0]);
  int e2 = ElemNum(loa[1]);
  //Cuenta la concentracion atomica de cada especie atomica.
  int ne1=0,ne2=0;
  for(int m=0;m<N;m++)
  {
   if(atoms[m].Symbol()==loa[0]) ne1++;
   if(atoms[m].Symbol()==loa[1]) ne2++;
  }
  double ce1 = (double)ne1/(double)N;
  double ce2 = (double)ne2/(double)N;
  //Comienza la asignacion principal para g(r) total.
  for(int i=0;i<nb;i++)
  {
   if(e1==e2) gt[i] = gt[i]+ce1*ce2*g[i][j];
   else {gt[i]=gt[i]+2*ce1*ce2*g[i][j];}
  }
  j++;
 }
 //
 // Output of g(r)
 //
 lpmd::Matrix *m=NULL;
 m = new lpmd::Matrix(2 + nsp*(nsp+1)/2, nb);

 // Asigna los labels al objeto Matrix para cada columna
 m->SetLabel(0, "r");
 m->SetLabel(nsp*(nsp+1)/2+1, "total g(r)");
 j=1;
 for (long int i = 0 ; i < pairs.Size() ; ++i)
 {
  m->SetLabel(j, pairs[i]+" g(r)");
  j++;
 }
 // 
 for(int i=0;i<nb;i++)
 {
  m->Set(0, i, dr*i);
  for(int j=0;j<(int)(nsp*(nsp+1)/2);j++)
  {
   m->Set(j+1, i, g[i][j]);
  }
  m->Set(nsp*(nsp+1)/2+1, i, gt[i]);
 }
 delete [] gt;
 for (int i=0;i<nb;i++) delete [] g[i];
 delete [] g;
 return m;
}
