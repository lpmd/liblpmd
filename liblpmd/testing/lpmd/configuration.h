/*
 *
 *
 *
 */

#ifndef __LPMD_CONFIGURATION_H__
#define __LPMD_CONFIGURATION_H__

#include <lpmd/basicparticleset.h>
#include <lpmd/basiccell.h>
#include <lpmd/cellmanager.h>

namespace lpmd
{
 class Configuration
 {
  public:
    Configuration(): cellman(0) { };
    virtual ~Configuration() { };

    virtual BasicParticleSet & Atoms() = 0; 
    virtual const BasicParticleSet & Atoms() const = 0;
    virtual BasicCell & Cell() = 0;
    virtual const BasicCell & Cell() const = 0;

    void SetCellManager(lpmd::CellManager & cm) { cellman = &cm; }

    lpmd::CellManager & CellManager() { return (*cellman); }

    NeighborList & Neighbors(long int i, bool full, double rcut)
    {
     cellman->BuildNeighborList(*this, i, neighlist, full, rcut);
     return neighlist;
    }

  private:
    lpmd::CellManager * cellman;
    NeighborList neighlist;
 };
}  // lpmd

#endif

