/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include <iostream>
#include <lpmd/matrix.h>
#include "py_matrix.h"

//
// Allocator
//
extern "C" PyObject * PyMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PyMatrix * self;
 self = (PyMatrix *)(type->tp_alloc(type, 0));
 if (self != NULL) self->matrix = NULL;
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PyMatrix_dealloc(PyMatrix * self)
{
 delete self->matrix;
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PyMatrix_init(PyMatrix * self, PyObject *args, PyObject *kwds)
{
 static char * kwlist[] = { (char *)("ncols"), (char *)("nrows"), NULL};
 int ncols, nrows;
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii", kwlist, &ncols, &nrows)) return -1;
 self->matrix = new lpmd::Matrix(ncols, nrows);
 return 0;
}

static PyObject * PyMatrix_Get(PyMatrix * self, PyObject * args)
{
 int col, row;
 PyArg_ParseTuple(args, "ii", &col, &row);
 PyObject * res = PyFloat_FromDouble((self->matrix)->Get(col, row));
 Py_INCREF(res);
 return res;
}

static PyObject * PyMatrix_GetLabel(PyMatrix * self, PyObject * args)
{
 int col;
 PyArg_ParseTuple(args, "i", &col);
 PyObject * res = Py_BuildValue("s", (self->matrix)->GetLabel(col));
 Py_INCREF(res);
 return res;
}

//
//
static PyObject * PyMatrix_getsize(PyMatrix * self, void * closure)
{
 return Py_BuildValue("(ii)", self->matrix->Columns(), self->matrix->Rows());
}

//
//
//
static PyGetSetDef PyMatrix_getsetters[] = {
    { (char *)("size"), (getter)PyMatrix_getsize, NULL, (char *)("Current size"), NULL},
    {NULL}  /* Sentinel */
};
 
//
//
//
static PyMethodDef PyMatrix_methods[] = {
    { "Get", (PyCFunction)PyMatrix_Get, METH_VARARGS, "Gets the value at the given position." },
    { "GetLabel", (PyCFunction)PyMatrix_GetLabel, METH_VARARGS, "Gets the label for the given column." },
    {NULL}  /* Sentinel */
};

/*
 *
 */
PyTypeObject PyMatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.Matrix",                  /* tp_name */
    sizeof(PyMatrix),               /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyMatrix_dealloc,   /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Matrix objects",               /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyMatrix_methods,               /* tp_methods */
    0,                              /* tp_members */
    PyMatrix_getsetters,            /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyMatrix_init,        /* tp_init */
    0,                              /* tp_alloc */
    PyMatrix_new,                   /* tp_new */
};

