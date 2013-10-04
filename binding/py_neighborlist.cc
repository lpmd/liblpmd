/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include <iostream>
#include <lpmd/neighborlist.h>
#include "py_neighborlist.h"

//
// Allocator
//
extern "C" PyObject * PyNeighborList_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PyNeighborList * self;
 self = (PyNeighborList *)type->tp_alloc(type, 0);
 if (self != NULL) self->nlist = NULL;
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PyNeighborList_dealloc(PyNeighborList * self)
{
 delete self->nlist;
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PyNeighborList_init(PyNeighborList * self, PyObject *args, PyObject *kwds)
{
 static char * kwlist[] = { (char *)("size"), NULL};
 int n = -1;
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &n)) return -1;
 if (n >= 0) self->nlist = new lpmd::NeighborList(n);
 else self->nlist = new lpmd::NeighborList();
 return 0;
}

static PyObject * PyNeighborList_Resize(PyNeighborList * self, PyObject * args)
{
 int s=0;
 PyArg_ParseTuple(args, "i", &s);
 self->nlist->Resize(s);
 Py_RETURN_NONE;
}

static PyObject * PyNeighborList_getsize(PyNeighborList * self, void * closure)
{
 return Py_BuildValue("i", self->nlist->Size());
}

static Py_ssize_t PyNeighborList_len(PyObject * self) { return reinterpret_cast<PyNeighborList *>(self)->nlist->Size(); }

static PyObject * PyNeighborList_getitem(PyObject * obj, Py_ssize_t i)
{
 PyNeighborList * self = reinterpret_cast<PyNeighborList *>(obj);
 const lpmd::AtomPair & ap = (*(self->nlist))[i];
 return Py_BuildValue("((ddd)dllllO)", ap.dr[0], ap.dr[1], ap.dr[2], ap.r2, ap.index_a, ap.index_b, ap.gindex_a, ap.gindex_b, (ap.jlocal ? Py_True : Py_False ));
}

static int PyNeighborList_setitem(PyObject * obj, Py_ssize_t i, PyObject * v)
{
 lpmd::AtomPair ap;
 PyNeighborList * self = reinterpret_cast<PyNeighborList *>(obj);
 PyObject * bobj = NULL, * drobj = NULL;
 PyArg_Parse(v, "(OdllllO)", &drobj, &(ap.r2), &(ap.index_a), &(ap.index_b), &(ap.gindex_a), &(ap.gindex_b), &bobj);
 Py_INCREF(bobj);
 Py_INCREF(drobj);
 PyArg_Parse(drobj, "(ddd)", ap.dr, ap.dr+1, ap.dr+2);
 Py_XDECREF(drobj);
 ap.jlocal = (bobj == Py_True);
 Py_XDECREF(bobj);
 (*(self->nlist))[i] = ap;
 return 0;
}

//
//
//
static PyGetSetDef PyNeighborList_getsetters[] = {
    { (char *)("size"), (getter)PyNeighborList_getsize, NULL, (char *)("Current size"), NULL},
    {NULL}  /* Sentinel */
};
 
//
//
//
static PyMethodDef PyNeighborList_methods[] = {
    { "Resize", (PyCFunction)PyNeighborList_Resize, METH_VARARGS, "Changes the size of the NeighborList." },
    {NULL}  /* Sentinel */
};

static PySequenceMethods PyNeighborList_sequence_methods = {
   PyNeighborList_len,              /* sq_length */
   NULL,
   NULL,
   PyNeighborList_getitem,          /* sq_item */
   NULL,
   PyNeighborList_setitem,          /* sq_ass_item */
};

/*
 *
 */
PyTypeObject PyNeighborListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.NeighborList",                /* tp_name */
    sizeof(PyNeighborList),         /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyNeighborList_dealloc, /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    &PyNeighborList_sequence_methods, /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "NeighborList objects",         /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyNeighborList_methods,         /* tp_methods */
    0,                              /* tp_members */
    PyNeighborList_getsetters,      /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyNeighborList_init,  /* tp_init */
    0,                              /* tp_alloc */
    PyNeighborList_new,             /* tp_new */
};

