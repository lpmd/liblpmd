/*
 *
 *
 *
 */

#ifndef __PY_ORTHOCELL_H__
#define __PY_ORTHOCELL_H__

#include <lpmd/orthogonalcell.h>
#include <lpmd/nonorthogonalcell.h>

extern "C" PyTypeObject PyCellType;

//
//
class PyCell
{
 public:
   PyObject_HEAD
   lpmd::BasicCell * bcell;
};


#endif

