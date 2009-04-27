/*
 *
 *
 *
 */

#ifndef __LPMD_ATOM_H__
#define __LPMD_ATOM_H__

#include <cstdlib>
#include <string>
#include "vector3.h"
#include "atominterface.h"
#include "elements.h"

class Atom: public AtomInterface
{
 public: 
  Atom(): AtomInterface(0), index(0) 
  { 
   iv = new Vector3[3]; 
  }

  Atom(const std::string a): AtomInterface(ElemNum(a))
  {
   iv = new Vector3[3]; 
  }

  Atom(const int zeta): AtomInterface(zeta)
  {
   iv = new Vector3[3];
  }

  Atom(const std::string a, Vector & pos): AtomInterface(ElemNum(a))
  {
   iv = new Vector3[3];
   iv[0] = pos;
  }

  Atom(const std::string a, Vector & pos, Vector & vel): AtomInterface(ElemNum(a))
  {
   iv = new Vector3[3];
   iv[0] = pos;
   iv[1] = vel;
  }
 
  Atom(const std::string elem, Vector & pos, Vector & vel, Vector & ace): AtomInterface(ElemNum(elem))
  {
   iv = new Vector3[3];
   iv[0] = pos;
   iv[1] = vel;
   iv[2] = ace;
  }

  ~Atom() 
  {
   delete [] iv;
  } 

 private:
  long index; //index atom
};

//
//
//


#endif

