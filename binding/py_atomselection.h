/*
 *
 *
 *
 */

#ifndef __PY_ATOMSELECTION_H__
#define __PY_ATOMSELECTION_H__

extern "C" PyTypeObject PyAtomSelectionType;
extern "C" PyObject * PyAtomSelection_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

#include <lpmd/atomselection.h>

//
//
class PyAtomSelection
{
 public:
   PyObject_HEAD
   lpmd::AtomSelection * asel;
};

#endif

