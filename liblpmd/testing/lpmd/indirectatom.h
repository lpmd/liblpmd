/*
 *
 *
 *
 */

#ifndef __LPMD_INDIRECT_ATOM_H__
#define __LPMD_INDIRECT_ATOM_H__

#include <lpmd/atominterface.h>

namespace lpmd
{

class IndirectAtom: public AtomInterface
{
 public:
   IndirectAtom(): AtomInterface(0, 0, 0, 0) {  }
   IndirectAtom(int z): AtomInterface(z, 0, 0, 0) {  }
   ~IndirectAtom() {  }
  
   inline void SetAddresses(Vector * pos, Vector * vel, Vector * acc) 
   { 
    ipos = pos;
    ivel = vel;
    iacc = acc;
   }
};

}

#endif

