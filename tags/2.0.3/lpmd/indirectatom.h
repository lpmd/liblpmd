/*
 *
 *
 *
 */

#ifndef __LPMD_INDIRECT_ATOM_H__
#define __LPMD_INDIRECT_ATOM_H__

#include <lpmd/basicatom.h>

namespace lpmd
{

class IndirectAtom: public BasicAtom
{
 public:
   IndirectAtom(): BasicAtom(0, 0, 0, 0) {  }
   IndirectAtom(int z): BasicAtom(z, 0, 0, 0) {  }
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

