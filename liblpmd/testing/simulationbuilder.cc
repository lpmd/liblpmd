/*
 *
 *
 *
 */

#include <lpmd/simulationbuilder.h>
#include <lpmd/nonorthogonalcell.h>
#include <lpmd/orthogonalcell.h>
#include <lpmd/particleset.h>
#include <lpmd/fixedsizeparticleset.h>
#include <lpmd/combinedpotential.h>
#include <lpmd/integrator.h>
#include <lpmd/properties.h>
#include <lpmd/session.h>

using namespace lpmd;

template <typename AtomContainer=lpmd::ParticleSet, typename CellType=lpmd::Cell> class SimulationEngine: public Simulation
{
 public:
  SimulationEngine(): atoms(0), cell(0), integ(0), velocitiesSet(false), initialized(false), step(0)
  {
   // This hints SimulationEngine that the number of atoms is variable
   atoms = new AtomContainer();  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
   SetTag(*this, Tag("step"), step);
  }

  SimulationEngine(long int natoms): atoms(0), cell(0), integ(0), velocitiesSet(false), initialized(false), step(0)
  {
   // This fixes the number of atoms for SimulationEngine
   atoms = new AtomContainer(natoms);  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
   SetTag(*this, Tag("step"), step);
  }

  SimulationEngine(long int natoms, const BasicAtom & t): atoms(0), cell(0), integ(0), velocitiesSet(false), initialized(false), step(0)
  {
   // This fixes the number of atoms for SimulationEngine
   atoms = new AtomContainer(natoms, t);  // maybe ParticleSet, or another type of ParticleSet
   cell = new CellType();
   SetTag(*this, Tag("step"), step);
  }

  ~SimulationEngine()
  {
   delete atoms;
   delete cell;
  }

 void SetTemperature(double temp) 
 {  
  if (! velocitiesSet) InitVelocities();
  BasicParticleSet & atomaccesor = Atoms(); // en vez de (*atoms), para que funcione con ApplyAtomMask
  double ti = Temperature(atomaccesor);
  for (long int i=0;i<atomaccesor.Size();++i) atomaccesor[i].Velocity() *= sqrt(temp/ti);
 }

 CellType & OriginalCell() { return (*cell); }
 const CellType & OriginalCell() const { return (*cell); }

 AtomContainer & OriginalAtoms() { return (*atoms); }
 const AtomContainer & OriginalAtoms() const { return (*atoms); }

 CombinedPotential & Potentials() { return potarray; }
 
 const CombinedPotential & Potentials() const { return potarray; }

 void SetSimulationTags()
 {
  const double pressfactor = double(GlobalSession["pressfactor"]);
  const double v = Cell().Volume();
  const Vector P = Momentum(Atoms());
  SetTag(*this, Tag("step"), step);
  SetTag(*this, Tag("temperature"), Temperature(Atoms()));
  SetTag(*this, Tag("volume"), v);
  SetTag(*this, Tag("volume-per-atom"), v/double(Atoms().Size()));
  SetTag(*this, Tag("cell-a"), Cell()[0].Module());
  SetTag(*this, Tag("cell-b"), Cell()[1].Module());
  SetTag(*this, Tag("cell-c"), Cell()[2].Module());
  SetTag(*this, Tag("particle-density"), double(Atoms().Size())/v);
  SetTag(*this, Tag("density"), Density(Atoms(), Cell()));
  SetTag(*this, Tag("momentum"), P.Module());
  SetTag(*this, Tag("px"), P[0]);
  SetTag(*this, Tag("py"), P[1]);
  SetTag(*this, Tag("pz"), P[2]);
  double potenerg = double(Parameter(GetTag(*this, Tag("potential-energy"))));
  double kinenerg = KineticEnergy(Atoms());
  SetTag(*this, Tag("kinetic-energy"), kinenerg);
  SetTag(*this, Tag("total-energy"), potenerg+kinenerg);
  const Matrix & stress = StressTensor();
  SetTag(*this, Tag("virial-pressure"), (pressfactor/v)*(1.0/3.0)*Virial());
  SetTag(*this, Tag("kinetic-pressure"), (pressfactor/v)*(2.0/3.0)*kinenerg);
  SetTag(*this, Tag("pressure"), (pressfactor/v)*((2.0/3.0)*kinenerg+(1.0/3.0)*Virial()));

  char component_letter[3] = {'x', 'y', 'z'};
  for (int p=0;p<3;++p)
   for (int q=0;q<3;++q)
   {
    std::string label = "s"+component_letter[q]+component_letter[p];
    SetTag(*this, Tag(label), (pressfactor/v)*stress.Get(q, p));
   }
 }
 
 void DoStep()
 {  
  if (! initialized) Initialize();
  integ->Advance(*this, potarray);
  step++;
  SetSimulationTags();
 }
 
 inline void DoSteps(long int n) { for (int q=0;q<n;++q) DoStep(); }

 long int CurrentStep() const { return step; }

 void SetIntegrator(lpmd::Integrator & itg) { integ = &itg; }

 lpmd::Integrator & Integrator() 
 { 
  if (integ == 0) throw MissingComponent("integrator");
  return (*integ); 
 }

 void RescalePositions(const BasicCell & old_cell)
 {
  BasicParticleSet & t_atoms = Atoms();
  BasicCell & t_cell = Cell();
  for (long int i=0;i<t_atoms.Size();++i)
      t_atoms[i].Position() = t_cell.Cartesian(old_cell.Fractional(t_atoms[i].Position()));
 }
 
 private:
  void InitVelocities() 
  { 
   Vector vel, totalp;
   long nparts = atoms->Size();
   double targettemp = 1.0;
   double Kpart = (3.0/2.0)*kboltzmann*targettemp;
   for (int i=0;i<nparts;++i)
   {
    double v2 = 2.0*Kpart/(*atoms)[i].Mass();
    vel = RandomVector(sqrt(v2));
    totalp += vel*(*atoms)[i].Mass();
    (*atoms)[i].Velocity() = vel;
   }
   totalp = totalp/nparts;
   for (int i=0;i<nparts;++i)
   {
    double mass = (*atoms)[i].Mass();
    (*atoms)[i].Velocity() -= (totalp/mass);
   }
  }

  void Initialize() 
  { 
   integ->Initialize(*this, potarray); 
   step = 0;
   SetTag(*this, Tag("step"), step);
   initialized = true;
  }

  AtomContainer * atoms;
  CellType * cell;
  lpmd::CombinedPotential potarray;
  lpmd::Integrator * integ;
  bool velocitiesSet;
  bool initialized;
  long int step;
};

//
//
//
class lpmd::SimBuildImpl
{
 public:
   Array<Simulation *> s_array;
};

lpmd::SimBuildImpl SimulationBuilder::impl = SimBuildImpl();

SimulationBuilder::SimulationBuilder() { }

SimulationBuilder::~SimulationBuilder()
{
 for (long int i=0;i<impl.s_array.Size();++i) delete impl.s_array[i];
}

Simulation & SimulationBuilder::CreateFixedOrthogonal(long int atoms, const BasicAtom & at)
{
 Simulation * simp = new SimulationEngine<FixedSizeParticleSet, OrthogonalCell>(atoms, at);
 impl.s_array.Append(simp);
 return (*simp);
}

Simulation & SimulationBuilder::CreateGeneric(long int atoms, const BasicAtom & at)
{
 Simulation * simp = new SimulationEngine<ParticleSet, NonOrthogonalCell>(atoms, at);
 impl.s_array.Append(simp);
 return (*simp);
}

Simulation & SimulationBuilder::CreateGeneric()
{
 Simulation * simp = new SimulationEngine<ParticleSet, NonOrthogonalCell>();
 impl.s_array.Append(simp);
 return (*simp);
}

Simulation & SimulationBuilder::CloneOptimized(const Simulation & sim)
{
 const BasicParticleSet & atoms = sim.Atoms();
 const BasicCell & cell = sim.Cell();
 Simulation * simp = 0;
 if (cell.IsOrthogonal()) 
 {
  simp = new SimulationEngine<FixedSizeParticleSet, OrthogonalCell>(atoms.Size(), atoms[0]);
 }
 else
 {
  simp = new SimulationEngine<FixedSizeParticleSet, NonOrthogonalCell>(atoms.Size(), atoms[0]);
 }

 // Se copia como FixedSize en caso de que sean realmente muchos atomos...
 FixedSizeParticleSet & newatoms = reinterpret_cast<FixedSizeParticleSet&>(simp->Atoms());

 BasicCell & newcell = simp->Cell();
 for (long int i=0;i<atoms.Size();++i) newatoms[i] = atoms[i];
 for (int q=0;q<3;++q) newcell[q] = cell[q];
 impl.s_array.Append(simp);
 return (*simp);
}

