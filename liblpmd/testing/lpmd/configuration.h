/*
 *
 *
 *
 */

#ifndef __LPMD_CONFIGURATION_H__
#define __LPMD_CONFIGURATION_H__

#include <lpmd/basicparticleset.h>
#include <lpmd/basiccell.h>
#include <lpmd/taghandler.h>
#include <lpmd/cellmanager.h>
#include <lpmd/matrix.h>

namespace lpmd
{
 class Configuration: public TagHandler<Configuration>
 {
  public:
    Configuration(): cellman(0), stresstensor(3, 3) { }
    virtual ~Configuration() { };

    virtual BasicParticleSet & Atoms() = 0; 
    virtual const BasicParticleSet & Atoms() const = 0;
    virtual BasicCell & Cell() = 0;
    virtual const BasicCell & Cell() const = 0;

    void ShowInfo(std::ostream & out);

    double & Virial() { return virial; }
    const double & Virial() const { return virial; }

    Matrix & StressTensor() { return stresstensor; }
    const Matrix & StressTensor() const { return stresstensor; }

    void SetCellManager(lpmd::CellManager & cm) 
    { 
     cellman = &cm; 
     cm.UpdateCell(*this);
    }

    lpmd::CellManager & GetCellManager() 
    {
     if (cellman == 0) throw MissingComponent("cellmanager");
     return (*cellman); 
    }

    NeighborList & Neighbors(long int i, bool full, double rcut)
    {
     cellman->BuildNeighborList(*this, i, neighlist, full, rcut);
     return neighlist;
    }

    double MinimumPairDistance() const;

  private:
    lpmd::CellManager * cellman;
    NeighborList neighlist;
    double virial;
    Matrix stresstensor;
 };
}  // lpmd

#endif

