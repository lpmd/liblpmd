/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include <iostream>
#include <lpmd/atomselection.h>
#include "py_atomselection.h"

//
// Allocator
//
extern "C" PyObject * PyAtomSelection_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PyAtomSelection * self;
 self = (PyAtomSelection *)type->tp_alloc(type, 0);
 if (self != NULL) self->asel = NULL;
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PyAtomSelection_dealloc(PyAtomSelection * self)
{
 delete self->asel;
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PyAtomSelection_init(PyAtomSelection * self, PyObject *args, PyObject *kwds)
{
 static char * kwlist[] = { (char *)("size"), NULL};
 int n = -1;
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &n)) return -1;
 if (n >= 0) self->asel = new lpmd::AtomSelection(n);
 else self->asel = new lpmd::AtomSelection();
 return 0;
}

static PyObject * PyAtomSelection_getsize(PyAtomSelection * self, void * closure)
{
 return Py_BuildValue("i", self->asel->Size());
}

static Py_ssize_t PyAtomSelection_len(PyObject * self) { return reinterpret_cast<PyAtomSelection *>(self)->asel->Size(); }

static PyObject * PyAtomSelection_getitem(PyObject * obj, Py_ssize_t i)
{
 PyAtomSelection * self = reinterpret_cast<PyAtomSelection *>(obj);
 PyObject * res = PyLong_FromLong((*(self->asel))[i]);
 Py_INCREF(res);
 return res;
}

//
//
//
static PyGetSetDef PyAtomSelection_getsetters[] = {
    { (char *)("size"), (getter)PyAtomSelection_getsize, NULL, (char *)("Current size"), NULL},
    {NULL}  /* Sentinel */
};
 
//
//
//
static PyMethodDef PyAtomSelection_methods[] = {
    {NULL}  /* Sentinel */
};

static PySequenceMethods PyAtomSelection_sequence_methods = {
   PyAtomSelection_len,              /* sq_length */
   NULL,
   NULL,
   PyAtomSelection_getitem,          /* sq_item */
   NULL,
   NULL,                             /* sq_ass_item */
};

/*
 *
 */
PyTypeObject PyAtomSelectionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.AtomSelection",                /* tp_name */
    sizeof(PyAtomSelection),         /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyAtomSelection_dealloc, /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    &PyAtomSelection_sequence_methods, /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "AtomSelection objects",         /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyAtomSelection_methods,         /* tp_methods */
    0,                              /* tp_members */
    PyAtomSelection_getsetters,      /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyAtomSelection_init,  /* tp_init */
    0,                              /* tp_alloc */
    PyAtomSelection_new,             /* tp_new */
};

