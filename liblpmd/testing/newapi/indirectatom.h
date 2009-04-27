/*
 *
 *
 *
 */

#ifndef __LPMD_INDIRECT_ATOM_H__
#define __LPMD_INDIRECT_ATOM_H__

#include "atominterface.h"

class IndirectAtom: public AtomInterface
{
 public:
   IndirectAtom(): AtomInterface(0) {  }
   IndirectAtom(int z): AtomInterface(z) {  }
   ~IndirectAtom() {  }
  
   inline void SetAddress(Vector * p) { iv = p; }
};

#endif

