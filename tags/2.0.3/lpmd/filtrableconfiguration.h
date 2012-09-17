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
     if (innerselect == 0) return OriginalAtoms();
     if (selector_inverted) 
     {
      BasicParticleSet & temp = const_cast<BasicParticleSet &>(innerselect->InverseSelectFrom(OriginalAtoms()));
      static_cast< TagHandler<BasicAtom> & >(temp) = static_cast<const TagHandler<BasicAtom> &>(OriginalAtoms());
      return temp;
     }
     else 
     {
      BasicParticleSet & temp = const_cast<BasicParticleSet &>(innerselect->SelectFrom(OriginalAtoms()));
      static_cast< TagHandler<BasicAtom> & >(temp) = static_cast<const TagHandler<BasicAtom> &>(OriginalAtoms());
      return temp;
     }
    }

    inline const BasicParticleSet & Atoms() const 
    { 
     if (innerselect == 0) return OriginalAtoms();
     if (selector_inverted) 
     {
      BasicParticleSet & temp = const_cast<BasicParticleSet &>(innerselect->InverseSelectFrom(OriginalAtoms()));
      static_cast< TagHandler<BasicAtom> & >(temp) = static_cast<const TagHandler<BasicAtom> &>(OriginalAtoms());
      return temp;
     }
     else 
     {
      BasicParticleSet & temp = const_cast<BasicParticleSet &>(innerselect->SelectFrom(OriginalAtoms()));
      static_cast< TagHandler<BasicAtom> & >(temp) = static_cast<const TagHandler<BasicAtom> &>(OriginalAtoms());
      return temp;
     }
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

