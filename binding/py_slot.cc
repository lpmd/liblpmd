/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include <iostream>

#include "py_slot.h"
#include "py_atomset.h"
#include "py_cell.h"
#include "py_neighborlist.h"
#include "py_matrix.h"
#include "py_atomselection.h"

#include <lpmd/atomset.h>
#include <lpmd/basiccell.h>
#include <lpmd/matrix.h>

//
// Allocator
//
extern "C" PyObject * PySlot_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PySlot * self;
 self = (PySlot *)type->tp_alloc(type, 0);
 if (self != NULL) 
 { 
  self->funcptr = NULL; 
  self->setkw = NULL; 
  self->is_mask = false;
  self->inverse = false;
 }
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PySlot_dealloc(PySlot * self)
{
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PySlot_init(PySlot * self, PyObject *args, PyObject *kwds)
{
 PyObject * loc_str = NULL, * kwset_str = NULL;
 PyObject * ismask = NULL;
 PyObject * inverse = NULL;
 static char * kwlist[] = { (char *)("location"), (char *)("keywordsetter"), (char *)("ismask"), (char *)("inverse"), NULL};
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO|OO", kwlist, &loc_str, &kwset_str, &ismask, &inverse)) return -1;
 if ((ismask != NULL) && (ismask == Py_True)) self->is_mask = true;
 if ((inverse != NULL) && (inverse == Py_True)) self->inverse = true;
 PyObject * pystr = PyUnicode_AsEncodedString(loc_str, "ascii", "Error ~");
 Py_INCREF(pystr);
 const char * str = PyBytes_AS_STRING(pystr);
 void * ptr = NULL;
 sscanf(str, "%p", &ptr);
 if (ptr == NULL) return -1;
 self->funcptr = ptr;
 //
 pystr = PyUnicode_AsEncodedString(kwset_str, "ascii", "Error ~");
 Py_INCREF(pystr);
 str = PyBytes_AS_STRING(pystr);
 ptr = NULL;
 sscanf(str, "%p", &ptr);
 if (ptr == NULL) return -1;
 self->setkw = ptr;
 return 0;
}

PyObject * PySlot_call(PyObject * self, PyObject * args, PyObject * kwds)
{
 PyObject * sim = NULL, * params = NULL, * modify = NULL, * bound = NULL;
 static char * kwlist[] = { (char *)("simulation"), (char *)("parameters"), (char *)("modify"), 
                            (char *)("useboundaries"), NULL};
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO|OO", kwlist, &sim, &params, &modify, &bound))
 {
  //PyErr_SetString(PyExc_Exception, "No return value available");
  return NULL;
 }
 bool can_modify = false;
 bool use_boundaries = false;
 if (modify != NULL) can_modify = (modify == Py_True);
 if (bound != NULL) use_boundaries = (bound == Py_True);
 //
 int (*kwsetter)(const char *, const char *) = ( int (*)(const char *, const char *))(((PySlot *)(self))->setkw);
 PyObject * keys = PyObject_CallMethod(params, (char *)("keys"), NULL);
 PyObject * iterator = PyObject_GetIter(keys);
 PyObject * key;
 while ((key = PyIter_Next(iterator)))
 {
  PyObject * value = PyDict_GetItem(params, key);
  PyObject * pystr = PyUnicode_AsEncodedString(key, "ascii", "Error ~");
  const char * keystr = PyBytes_AS_STRING(pystr);
  PyObject * pystr2 = PyUnicode_AsEncodedString(value, "ascii", "Error ~");
  const char * valuestr = PyBytes_AS_STRING(pystr2);
  (*kwsetter)(keystr, valuestr);
 }
 void (*funcptr)(const RawAtomSet *, const RawCell *) = (void (*)(const RawAtomSet *, const RawCell *))(((PySlot *)(self))->funcptr);
 void (*funcptr2)(RawAtomSet *, RawCell *) = (void (*)(RawAtomSet *, RawCell *))(((PySlot *)(self))->funcptr);
 //
 // Retrieves the AtomSet 
 PyObject * aset = PyObject_GetAttr(sim, Py_BuildValue("s", "atomset"));
 if (aset == NULL) return NULL;
 _PyAtomSet * pyaset = (_PyAtomSet *)(aset);
 assert(pyaset != NULL);
 // Retrieves the BasicCell
 PyObject * bcell = PyObject_GetAttr(sim, Py_BuildValue("s", "cell"));
 if (bcell == NULL) return NULL;
 PyCell * pycell = (PyCell *)(bcell);
 assert(pycell != NULL);
 lpmd::BasicCell * basiccell = pycell->bcell;
 //
 // Executes the task safely, either as a regular task or as a mask application
 //
 if (((PySlot *)(self))->is_mask)
 {
  (pyaset->aset)->ApplyAtomMask(*funcptr, *basiccell, ((PySlot *)(self))->inverse);
  (pyaset->aset)->SetReturnType(-1); // the output value was already consumed by ApplyAtomMask
 }
 else
 {
  if (can_modify) (pyaset->aset)->RunTask(*funcptr2, *basiccell, use_boundaries);
  else (pyaset->aset)->RunTask(*funcptr, *basiccell, use_boundaries);
 }
 //
 //
 // Builds the returned object 
 int vtype = (pyaset->aset)->GetReturnType();
 if (vtype == -1) { Py_RETURN_NONE; }
 else
 {
  PyObject * pyvalue = NULL;
  if (vtype == RET_DOUBLE)
  {
   double retv = lpmd::GetTaskValue<double>(*(pyaset->aset));
   pyvalue = PyFloat_FromDouble(retv);
   Py_INCREF(pyvalue);
  }
  else if (vtype == RET_VECTOR)
  {
   const lpmd::Vector v = lpmd::GetTaskValue<lpmd::Vector>(*(pyaset->aset));
   pyvalue = Py_BuildValue("ddd", v[0], v[1], v[2]);
   Py_INCREF(pyvalue);
  }
  else if (vtype == RET_MATRIX)
  {
   const lpmd::Matrix m = lpmd::GetTaskValue<lpmd::Matrix>(*(pyaset->aset));
   pyvalue = PyMatrix_new(&PyMatrixType, NULL, NULL);
   ((PyMatrix *)(pyvalue))->matrix = new lpmd::Matrix(m);
   Py_INCREF(pyvalue);
  }
  else if (vtype == RET_NLIST)
  {
   const lpmd::NeighborList nlist = lpmd::GetTaskValue<lpmd::NeighborList>(*(pyaset->aset));
   pyvalue = PyNeighborList_new(&PyNeighborListType, NULL, NULL);
   ((PyNeighborList *)(pyvalue))->nlist = new lpmd::NeighborList(nlist);
   Py_INCREF(pyvalue);
  }
  else if (vtype == RET_ASELECT)
  {
   const lpmd::AtomSelection asel = lpmd::GetTaskValue<lpmd::AtomSelection>(*(pyaset->aset));
   pyvalue = PyAtomSelection_new(&PyAtomSelectionType, NULL, NULL);
   ((PyAtomSelection *)(pyvalue))->asel = new lpmd::AtomSelection(asel);
   Py_INCREF(pyvalue);
  }
  else
  {
   // FIXME: Throw exception
  }
  return pyvalue;
 }
}

static PyObject * PySlot_getismask(PySlot * self, void * closure)
{
 PyObject * res = Py_False;
 if (self->is_mask) res = Py_True;
 Py_INCREF(res);
 return res;
}

static int PySlot_setismask(PySlot * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 self->is_mask = (value == Py_True);
 return 0;
}

static PyObject * PySlot_getinverse(PySlot * self, void * closure)
{
 PyObject * res = Py_False;
 if (self->inverse) res = Py_True;
 Py_INCREF(res);
 return res;
}

static int PySlot_setinverse(PySlot * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 self->inverse = (value == Py_True);
 return 0;
}

//
//
//
static PyMethodDef PySlot_methods[] = {
    {NULL}  /* Sentinel */
};

//
//
//
static PyGetSetDef PySlot_getsetters[] = {
    { (char *)("ismask"), (getter)PySlot_getismask, (setter)PySlot_setismask, (char *)("True is the slot is a mask."), NULL},
    { (char *)("inverse"), (getter)PySlot_getinverse, (setter)PySlot_setinverse, (char *)("True if the mask must be inverted."), NULL},
    {NULL}  /* Sentinel */
};
 
/*
 *
 */
PyTypeObject PySlotType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.Slot",                    /* tp_name */
    sizeof(PySlot),                 /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PySlot_dealloc,     /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    PySlot_call,                    /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Slot objects",                 /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PySlot_methods,                 /* tp_methods */
    0,                              /* tp_members */
    PySlot_getsetters,              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PySlot_init,          /* tp_init */
    0,                              /* tp_alloc */
    PySlot_new,                     /* tp_new */
};

