/*
 *
 *
 *
 */

#include <lpmd/configuration.h>

using namespace lpmd;

double Configuration::MinimumPairDistance() const
{
 const BasicParticleSet & atoms = Atoms();
 const BasicCell & cell = Cell();
 double min=1.0e5;
 for(long i=0;i<atoms.Size()-1;++i)
  for(long j=i+1;j<atoms.Size();++j)
  {
   double dist = cell.Displacement(atoms[j].Position(), atoms[i].Position()).Module();
   if (dist < min) min = dist;
  }
 return min;
}

void Configuration::ShowInfo(std::ostream & out)
{
 Vector pos, cm(0.0, 0.0, 0.0);
 BasicParticleSet & atoms = Atoms();
 BasicCell & cell = Cell();
 double tmass = 0.0, xmin, xmax, ymin, ymax, zmin, zmax;
 xmin = xmax = atoms[0].Position()[0];
 ymin = ymax = atoms[0].Position()[1];
 zmin = zmax = atoms[0].Position()[2];
 for (long int j=0;j<atoms.Size();++j)
 {
  pos = atoms[j].Position();
  cm = cm + atoms[j].Mass()*pos;
  tmass += atoms[j].Mass();
  if (pos[0] < xmin) xmin = pos[0];
  if (pos[0] > xmax) xmax = pos[0];
  if (pos[1] < ymin) ymin = pos[1];
  if (pos[1] > ymax) ymax = pos[1];
  if (pos[2] < zmin) zmin = pos[2];
  if (pos[2] > zmax) zmax = pos[2];
 }
 cm = cm / tmass;
 out << " Number of atoms        : " << atoms.Size() << " atoms\n";
 for (int q=0;q<3;++q) 
 out << " Cell vector [" << q << "]        : " << cell[q] << '\n';
 out << " Cell Volume            : " << cell.Volume() << '\n'; 
 out << " Center of Mass         : " << cm << '\n'; 
 out << " X coordinate           : [ " << xmin << " , " << xmax << " ] (width: " << xmax-xmin << ")\n";
 out << " Y coordinate           : [ " << ymin << " , " << ymax << " ] (width: " << ymax-ymin << ")\n";
 out << " Z coordinate           : [ " << zmin << " , " << zmax << " ] (width: " << zmax-zmin << ")\n";
 out << " X displacement from CM : [ " << xmin-cm[0] << " , " << xmax-cm[0] << " ]\n";
 out << " Y displacement from CM : [ " << ymin-cm[1] << " , " << ymax-cm[1] << " ]\n";
 out << " Z displacement from CM : [ " << zmin-cm[2] << " , " << zmax-cm[2] << " ]\n";
 out << '\n';
}

