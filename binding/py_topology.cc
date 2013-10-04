/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include "py_topology.h"
#include "py_cell.h"

//
// Allocator
//
extern "C" PyObject * PyTopology_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PyTopology * self;
 self = (PyTopology *)type->tp_alloc(type, 0);
 if (self != NULL) self->top = NULL;
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PyTopology_dealloc(PyTopology * self)
{
 delete self->top;
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PyTopology_init(PyTopology * self, PyObject *args, PyObject *kwds)
{
 static char * kwlist[] = { (char *)("cell"), (char *)("grid"), NULL};
 int nx = 1, ny = 1;
 PyObject * cell = NULL, * grid = NULL;
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &cell, &grid)) return -1;
 Py_INCREF(cell);
 lpmd::BasicCell * real_cell = (reinterpret_cast<PyCell *>(cell))->bcell;
 PyArg_ParseTuple(grid, "ii", &nx, &ny);
 self->top = new lpmd::CellTopology(*real_cell, nx, ny);
 return 0;
}

static PyObject * PyTopology_PositionAlong(PyTopology * self, PyObject * args)
{
 int q=0;
 PyArg_ParseTuple(args, "i", &q);
 return Py_BuildValue("i", self->top->PositionAlong(q));
}

static PyObject * PyTopology_RanksAlong(PyTopology * self, PyObject * args)
{
 int q=0;
 PyArg_ParseTuple(args, "i", &q);
 return Py_BuildValue("i", self->top->RanksAlong(q));
}

static PyObject * PyTopology_Barrier(PyTopology * self)
{
 parallel::Barrier();
 Py_RETURN_NONE;
}

static PyObject * PyTopology_GatherFromAll(PyTopology * self, PyObject * args)
{
 int rank0 = -1, slen = -1;
 char * str = NULL;
 PyArg_ParseTuple(args, "iy#", &rank0, &str, &slen);
 memory::Buffer<char> ref(str, slen, false, false);
 memory::Buffer<char> * chbuf = self->top->GatherFromAll(rank0, ref);
 if (self->top->CurrentRank() == rank0)
 {
  PyObject * t = PyTuple_New(self->top->NumberOfRanks());
  for (int i=0;i<self->top->NumberOfRanks();++i)
  {
   PyTuple_SetItem(t, i, Py_BuildValue("y#", chbuf[i].RawBuffer(), chbuf[i].Size()));
  }
  delete [] chbuf;
  return t;
 }
 else { assert (chbuf == NULL); Py_RETURN_NONE; }
}

static PyObject * PyTopology_getrank(PyTopology * self, void * closure)
{
 return Py_BuildValue("i", self->top->CurrentRank());
}

static PyObject * PyTopology_getranks(PyTopology * self, void * closure)
{
 int nranks = self->top->NumberOfRanks();
 PyObject * range = PyList_New(nranks);
 for (int i=0;i<nranks;++i) PyList_SetItem(range, i, PyLong_FromLong(i));
 Py_INCREF(range);
 return range;
}

//
//
//
static PyGetSetDef PyTopology_getsetters[] = {
    { (char *)("rank"), (getter)PyTopology_getrank, NULL, (char *)("Current rank"), NULL},
    { (char *)("ranks"), (getter)PyTopology_getranks, NULL, (char *)("List of all ranks"), NULL},
    {NULL}  /* Sentinel */
};
 
//
//
//
static PyMethodDef PyTopology_methods[] = {
    { "PositionAlong", (PyCFunction)PyTopology_PositionAlong, METH_VARARGS, "Position in the grid." },
    { "RanksAlong", (PyCFunction)PyTopology_RanksAlong, METH_VARARGS, "Length of the grid." },
    { "Barrier", (PyCFunction)PyTopology_Barrier, METH_NOARGS, "Syncronizes all ranks." },
    { "GatherFromAll", (PyCFunction)PyTopology_GatherFromAll, METH_VARARGS, "Gathers a bytes object from all." },
    {NULL}  /* Sentinel */
};

/*
 *
 */
PyTypeObject PyTopologyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.Topology",                /* tp_name */
    sizeof(PyTopology),             /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyTopology_dealloc, /* tp_dealloc */
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
    "Topology objects",             /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyTopology_methods,             /* tp_methods */
    0,                              /* tp_members */
    PyTopology_getsetters,          /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyTopology_init,      /* tp_init */
    0,                              /* tp_alloc */
    PyTopology_new,                 /* tp_new */
};

//
//
//
static int argc = -1;
static char ** argv = NULL;

PyObject * PyInitializeCommunication(PyObject * self, PyObject * args)
{
 PyObject * pyargv = NULL;
 PyArg_ParseTuple(args, "O", &pyargv);
 argc = PyObject_Length(pyargv);
 argv = new char *[argc];
 for (int i=0;i<argc;++i)
 {
  PyObject * pystr = PyUnicode_AsEncodedString(PyObject_GetItem(pyargv, PyLong_FromLong(i)), "ascii", "Error ~");
  Py_INCREF(pystr);
  const char * str = PyBytes_AS_STRING(pystr);
  argv[i] = new char[strlen(str)];
  strncpy(argv[i], str, strlen(str));
 }
 int nprocs = parallel::InitializeCommunication(&argc, &argv);
 return PyLong_FromLong(nprocs);
}

PyObject * PyFinishCommunication(PyObject * self, PyObject * args)
{
 parallel::FinishCommunication();
 for (int i=0;i<argc;++i) delete [] argv[i];
 delete [] argv;
 Py_RETURN_NONE;
}

