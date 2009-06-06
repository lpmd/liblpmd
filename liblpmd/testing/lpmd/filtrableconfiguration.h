/*
 *
 *
 *
 */

#ifndef __LPMD__FILTRABLECONFIGURATION_H__
#define __LPMD__FILTRABLECONFIGURATION_H__

#include <lpmd/configuration.h>
#include <lpmd/atomselector.h>

namespace lpmd
{
 class FiltrableConfiguration: public Configuration
 {
  public:
    FiltrableConfiguration(): innerselect(0) { }
    virtual ~FiltrableConfiguration() { }

    inline void ApplyAtomMask(Selector<BasicParticleSet> & selector) { innerselect = &selector; }
    inline void RemoveAtomMask() { innerselect = 0; }

    inline BasicParticleSet & Atoms()
    {
     return ((innerselect == 0) ? OriginalAtoms() : const_cast<BasicParticleSet&>(innerselect->SelectFrom(OriginalAtoms())));
    }

    inline const BasicParticleSet & Atoms() const 
    { 
     return ((innerselect == 0) ? OriginalAtoms() : innerselect->SelectFrom(OriginalAtoms()));
    }

    inline BasicCell & Cell() { return OriginalCell(); }
    inline const BasicCell & Cell() const { return OriginalCell(); }

    virtual BasicParticleSet & OriginalAtoms() = 0;
    virtual const BasicParticleSet & OriginalAtoms() const = 0;
    virtual BasicCell & OriginalCell() = 0;
    virtual const BasicCell & OriginalCell() const = 0;

  private:
    Selector<BasicParticleSet> * innerselect;
 };

}  // lpmd

#endif

