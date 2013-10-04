/*
 *
 *
 *
 */

#ifndef __PYATOMSET_H__
#define __PYATOMSET_H__

#include <lpmd/atomset.h>
#include <lpmd/neighborlist.h>
#include "py_neighborlist.h"

extern "C" PyTypeObject PyAtomSetType;

//
//
class _PyAtomSet
{
 public:
   PyObject_HEAD
   lpmd::AtomSet * aset;
   PyNeighborList * nlist;
};

#endif

