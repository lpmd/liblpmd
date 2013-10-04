/*
 *
 *
 */

#include <Python.h>
#include "py_topology.h"
#include "py_atomset.h"
#include "py_neighborlist.h"
#include <lpmd/error.h>

//
// Allocator
//

extern "C" PyObject * AtomSet_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 _PyAtomSet * self;
 self = (_PyAtomSet *)type->tp_alloc(type, 0);
 if (self != NULL) 
 {
  self->aset = NULL;
  self->nlist = NULL;
 }
 return (PyObject *)self;
}

//
//
//
extern "C" void AtomSet_dealloc(_PyAtomSet * self)
{
 delete self->aset;
 if (self->nlist != NULL) Py_DECREF(self->nlist);
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//
//
static PyObject * AtomSet_Clear(_PyAtomSet * self)
{
 self->aset->Clear();
 Py_RETURN_NONE;
}

static PyObject * AtomSet_HasReturnValue(_PyAtomSet * self)
{
 PyObject * res = ((self->aset->HasReturnValue()) ? Py_True : Py_False );
 Py_INCREF(res);
 return res;
}

static PyObject * AtomSet_GetReturnValue(_PyAtomSet * self)
{
 if (!(self->aset->HasReturnValue())) 
    PyErr_SetString(PyExc_Exception, "No return value available");
 Py_RETURN_NONE;
}

static PyObject * AtomSet_TaskTime(_PyAtomSet * self)
{
 PyObject * res = PyFloat_FromDouble(self->aset->TaskTime());
 Py_INCREF(res);
 return res;
}

static PyObject * AtomSet_Resize(_PyAtomSet * self, PyObject * args)
{
 int n=0;
 PyArg_ParseTuple(args, "i", &n);
 self->aset->Resize(n);
 Py_RETURN_NONE;
}

static PyObject * AtomSet_RunTask(_PyAtomSet * self, PyObject * args)
{
 PyObject * task = NULL, * use_boundaries = NULL;
 PyArg_ParseTuple(args, "O|O", &task, &use_boundaries);
 Py_XDECREF(task);
 if (use_boundaries)
 {
  Py_INCREF(use_boundaries);
  if (!PyBool_Check(use_boundaries))
  {
   Py_XDECREF(use_boundaries);
   PyErr_SetString(PyExc_ValueError, "Argument \"use_boundaries\" in AtomSet.RunTask must be True or False");
   Py_RETURN_NONE;
  }
  Py_XDECREF(use_boundaries);
 }
 Py_INCREF(task);
 PyObject_Print(task, stderr, Py_PRINT_RAW);
 PyObject * task_args = Py_BuildValue("(O)", self);
 Py_INCREF(task_args);
 PyObject_CallObject(task, task_args);
 Py_XDECREF(task_args);
 Py_XDECREF(task);
 Py_RETURN_NONE;
}

static PyObject * AtomSet_RemoveAtomMask(_PyAtomSet * self)
{
 self->aset->RemoveAtomMask();
 Py_RETURN_NONE;
}

extern "C" int AtomSet_init(_PyAtomSet * self, PyObject *args, PyObject *kwds)
{
 PyObject * managed = NULL, * topology = NULL, * useindex = NULL;
 static char *kwlist[] = {(char *)("managed"), (char *)("topology"), (char *)("useindex"), NULL};
 bool m = false;
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &managed, &topology, &useindex)) return -1; 
 if (managed) 
 {
  Py_INCREF(managed);
  if (!PyBool_Check(managed)) 
  {
   Py_XDECREF(managed);
   PyErr_SetString(PyExc_ValueError, "Argument \"managed\" in LPMD.AtomSet.__init__ must be True or False");
   return -1;
  }
  m = (managed == Py_True);
  Py_XDECREF(managed);
 }
 if (topology)
 {
  Py_INCREF(topology);
  lpmd::CellTopology * real_top = (reinterpret_cast<PyTopology *>(topology))->top;
  self->aset = new lpmd::AtomSet(*real_top, m);
 }
 else self->aset = new lpmd::AtomSet(m);
 return 0;
}

static PyObject * AtomSet_getnl(_PyAtomSet * self, void * closure)
{
 PyNeighborList * nlist = (self->nlist);
 Py_INCREF(nlist);
 return (PyObject *)(nlist);
}

static int AtomSet_setnl(_PyAtomSet * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 self->nlist = (PyNeighborList *)(value);
 lpmd::NeighborList * nl = (self->nlist)->nlist;
 (self->aset)->SetNeighborList(*nl);
 return 0;
}

static PyObject * AtomSet_getblength(_PyAtomSet * self, void * closure)
{
 PyObject * res = PyFloat_FromDouble((self->aset)->BoundaryLength());
 Py_INCREF(res);
 return res;
}

static int AtomSet_setblength(_PyAtomSet * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 double rc = PyFloat_AsDouble(value);
 try
 {
  (self->aset)->SetBoundaryLength(rc);
 }
 catch (lpmd::CutoffTooLarge & e)
 {
  PyErr_SetString(PyExc_RuntimeError, e.what());
  return -1;
 }
 return 0;
}

static PyObject * AtomSet_getlocalsize(_PyAtomSet * self, void * closure)
{
 PyObject * res = PyLong_FromLong((self->aset)->LocalSize());
 Py_INCREF(res);
 return res;
}

static PyObject * AtomSet_gettotalsize(_PyAtomSet * self, void * closure)
{
 PyObject * res = PyLong_FromLong((self->aset)->Size());
 Py_INCREF(res);
 return res;
}

//
//
//
static PyMethodDef AtomSet_methods[] = {
    { "Clear", (PyCFunction)AtomSet_Clear, METH_NOARGS, "Erases all atoms." },
    { "Resize", (PyCFunction)AtomSet_Resize, METH_VARARGS, "Changes the size of the AtomSet." },
    { "HasReturnValue", (PyCFunction)AtomSet_HasReturnValue, METH_NOARGS, "True if there is a return value set." },
    { "GetReturnValue", (PyCFunction)AtomSet_GetReturnValue, METH_NOARGS, "Retrieves the return value, if set." },
    { "TaskTime", (PyCFunction)AtomSet_TaskTime, METH_NOARGS, "Retrieves the time spent in tasks." },
    { "RunTask", (PyCFunction)AtomSet_RunTask, METH_VARARGS, "Executes a task." },
    { "RemoveAtomMask", (PyCFunction)AtomSet_RemoveAtomMask, METH_NOARGS, "Removes the current atom mask." },
    {NULL}  /* Sentinel */
};

static PyGetSetDef AtomSet_getsetters[] = {
    { (char *)("neighborlist"), (getter)AtomSet_getnl, (setter)AtomSet_setnl, (char *)("The current neighbors list."), NULL},
    { (char *)("localsize"), (getter)AtomSet_getlocalsize, NULL, (char *)("The current local size."), NULL},
    { (char *)("totalsize"), (getter)AtomSet_gettotalsize, NULL, (char *)("The total size."), NULL},
    { (char *)("boundarylength"), (getter)AtomSet_getblength, (setter)AtomSet_setblength, (char *)("The boundary length."), NULL},
    {NULL}  /* Sentinel */
};
/*
 *
 */
PyTypeObject PyAtomSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.AtomSet",            /* tp_name */
    sizeof(_PyAtomSet),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)AtomSet_dealloc,  /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "AtomSet objects",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    AtomSet_methods,           /* tp_methods */
    0,                         /* tp_members */
    AtomSet_getsetters,        /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)AtomSet_init,    /* tp_init */
    0,                         /* tp_alloc */
    AtomSet_new,               /* tp_new */
};

