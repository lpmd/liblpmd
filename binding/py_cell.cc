/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include "py_cell.h"

//
// Allocator
//
extern "C" PyObject * PyCell_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PyCell * self;
 self = (PyCell *)type->tp_alloc(type, 0);
 if (self != NULL) self->bcell = new lpmd::OrthogonalCell();
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PyCell_dealloc(PyCell * self)
{
 delete self->bcell;
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PyCell_init(PyCell * self, PyObject *args, PyObject *kwds)
{
 static char * kwlist[] = { (char *)("a"), (char *)("b"), (char *)("c"), 
                             (char *)("alpha"), (char *)("beta"), (char *)("gamma"), NULL};
 double cell_a = 1.0, cell_b = 1.0, cell_c = 1.0;
 double cell_alpha = 90.0, cell_beta = 90.0, cell_gamma = 90.0;
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "|dddddd", kwlist, &cell_a, &cell_b, &cell_c, &cell_alpha, &cell_beta, &cell_gamma)) return -1; 
 delete self->bcell;
 if (((fabs(cell_alpha-90.0) < 1.0E-08) && (fabs(cell_beta-90.0) < 1.0E-08)) && (fabs(cell_gamma-90.0) < 1.0E-08))
  self->bcell = new lpmd::OrthogonalCell(cell_a, cell_b, cell_c); 
 else 
  self->bcell = new lpmd::NonOrthogonalCell(cell_a, cell_b, cell_c, cell_alpha, cell_beta, cell_gamma); 
 return 0;
}

static PyObject * PyCell_IsOrthogonal(PyCell * self)
{
 PyObject * res;
 res = ((self->bcell->IsOrthogonal()) ? Py_True : Py_False );
 Py_INCREF(res);
 return res;
}

static PyObject * PyCell_Volume(PyCell * self)
{
 return Py_BuildValue("d", (self->bcell)->Volume());
}

static Py_ssize_t PyCell_len(PyObject * self) { return 3; }

static PyObject * PyCell_getitem(PyObject * obj, Py_ssize_t i)
{
 assert ((i >= 0) && (i <= 2));
 const lpmd::Vector basis_v = (*(reinterpret_cast<PyCell *>(obj)->bcell))[i];
 return Py_BuildValue("ddd", basis_v[0], basis_v[1], basis_v[2]); 
}

static int PyCell_setitem(PyObject * obj, Py_ssize_t i, PyObject * v)
{
 double a=0.0, b=0.0, c=0.0;
 assert ((i >= 0) && (i <= 2));
 PyArg_Parse(v, "(ddd)", &a, &b, &c);
 lpmd::Vector new_v(a, b, c);
 PyCell * self = reinterpret_cast<PyCell *>(obj);
 (*(self->bcell))[i] = new_v;
 const lpmd::Vector av = (*(self->bcell))[0];
 const lpmd::Vector bv = (*(self->bcell))[1];
 const lpmd::Vector cv = (*(self->bcell))[2];
 delete self->bcell;
 if (((Cross(av, bv).Module() < 1.0E-08) && (Cross(av, cv).Module() < 1.0E-08)) && (Cross(bv, cv).Module() < 1.0E-08))
  self->bcell = new lpmd::OrthogonalCell(av.Module(), bv.Module(), cv.Module());
 else 
  self->bcell = new lpmd::NonOrthogonalCell(av, bv, cv);
 return 0;
}

static PyObject * PyCell_getpx(PyCell * self, void * closure)
{
 PyObject * res;
 res = ( (self->bcell->Periodicity(0)) ? Py_True : Py_False );
 Py_INCREF(res);
 return res;
}

static int PyCell_setpx(PyCell * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 (self->bcell)->Periodicity(0) = (value == Py_True);
 Py_DECREF(value);
 return 0;
}

static PyObject * PyCell_getpy(PyCell * self, void * closure)
{
 PyObject * res;
 res = ( (self->bcell->Periodicity(1)) ? Py_True : Py_False );
 Py_INCREF(res);
 return res;
}

static int PyCell_setpy(PyCell * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 (self->bcell)->Periodicity(1) = (value == Py_True);
 Py_DECREF(value);
 return 0;
}

static PyObject * PyCell_getpz(PyCell * self, void * closure)
{
 PyObject * res;
 res = ( (self->bcell->Periodicity(2)) ? Py_True : Py_False );
 Py_INCREF(res);
 return res;
}

static int PyCell_setpz(PyCell * self, PyObject * value, void * closure)
{
 Py_INCREF(value);
 (self->bcell)->Periodicity(2) = (value == Py_True);
 Py_DECREF(value);
 return 0;
}

//
//
//
static PyGetSetDef PyCell_getsetters[] = {
    { (char *)("px"), (getter)PyCell_getpx, (setter)PyCell_setpx, (char *)("Periodicity in the X axis"), NULL},
    { (char *)("py"), (getter)PyCell_getpy, (setter)PyCell_setpy, (char *)("Periodicity in the Y axis"), NULL},
    { (char *)("pz"), (getter)PyCell_getpz, (setter)PyCell_setpz, (char *)("Periodicity in the Z axis"), NULL},
    {NULL}  /* Sentinel */
};
 
//
//
//
static PyMethodDef PyCell_methods[] = {
    { "IsOrthogonal", (PyCFunction)PyCell_IsOrthogonal, METH_NOARGS, "True if the cell is orthogonal." },
    { "Volume", (PyCFunction)PyCell_Volume, METH_NOARGS, "Returns the volume of the cell." },
    {NULL}  /* Sentinel */
};

static PySequenceMethods PyCell_sequence_methods = {
   PyCell_len,              /* sq_length */
   NULL,
   NULL,
   PyCell_getitem,          /* sq_item */
   NULL,
   PyCell_setitem,          /* sq_ass_item */
};

/*
 *
 */
PyTypeObject PyCellType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.Cell",          /* tp_name */
    sizeof(PyCell),              /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyCell_dealloc,  /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    &PyCell_sequence_methods,    /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash  */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Cell objects",       /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyCell_methods,              /* tp_methods */
    0,                              /* tp_members */
    PyCell_getsetters,           /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyCell_init,       /* tp_init */
    0,                              /* tp_alloc */
    PyCell_new,                  /* tp_new */
};

