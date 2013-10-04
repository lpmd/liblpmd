/*
 *
 *
 *
 */

#ifndef __PY_NEIGHBORLIST_H__
#define __PY_NEIGHBORLIST_H__

extern "C" PyTypeObject PyNeighborListType;
extern "C" PyObject * PyNeighborList_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

#include <lpmd/neighborlist.h>

//
//
class PyNeighborList
{
 public:
   PyObject_HEAD
   lpmd::NeighborList * nlist;
};

#endif

