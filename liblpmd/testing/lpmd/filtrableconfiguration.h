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
    FiltrableConfiguration(): innerselect(0), selector_inverted(false) { }
    virtual ~FiltrableConfiguration() { }

    inline void ApplyAtomMask(Selector<BasicParticleSet> & selector, bool inverted=false) 
    { 
     innerselect = &selector; 
     selector_inverted = inverted;
    }

    inline void RemoveAtomMask() { innerselect = 0; }

    inline BasicParticleSet & Atoms()
    {
     if (selector_inverted) 
        return ((innerselect == 0) ? OriginalAtoms() : const_cast<BasicParticleSet&>(innerselect->InverseSelectFrom(OriginalAtoms())));
     else return ((innerselect == 0) ? OriginalAtoms() : const_cast<BasicParticleSet&>(innerselect->SelectFrom(OriginalAtoms())));
    }

    inline const BasicParticleSet & Atoms() const 
    { 
     if (selector_inverted) 
        return ((innerselect == 0) ? OriginalAtoms() : innerselect->InverseSelectFrom(OriginalAtoms()));
     else return ((innerselect == 0) ? OriginalAtoms() : innerselect->SelectFrom(OriginalAtoms()));
    }

    inline BasicCell & Cell() { return OriginalCell(); }
    inline const BasicCell & Cell() const { return OriginalCell(); }

    virtual BasicParticleSet & OriginalAtoms() = 0;
    virtual const BasicParticleSet & OriginalAtoms() const = 0;
    virtual BasicCell & OriginalCell() = 0;
    virtual const BasicCell & OriginalCell() const = 0;

  private:
    Selector<BasicParticleSet> * innerselect;
    bool selector_inverted;
 };

}  // lpmd

#endif

