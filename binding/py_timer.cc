/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include "py_timer.h"

//
// Allocator
//
extern "C" PyObject * PyTimer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PyTimer * self;
 self = (PyTimer *)type->tp_alloc(type, 0);
 if (self != NULL) self->timer = new lpmd::Timer();
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PyTimer_dealloc(PyTimer * self)
{
 delete self->timer;
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PyTimer_init(PyTimer * self, PyObject *args, PyObject *kwds)
{
 if (self->timer != NULL) delete self->timer;
 self->timer = new lpmd::Timer(); 
 return 0;
}

static PyObject * PyTimer_Elapsed(PyTimer * self)
{
 PyObject * res;
 res = PyFloat_FromDouble(self->timer->Elapsed());
 Py_INCREF(res);
 return res;
}

static PyObject * PyTimer_Start(PyTimer * self)
{
 self->timer->Start();
 Py_RETURN_NONE;
}

static PyObject * PyTimer_Stop(PyTimer * self)
{
 self->timer->Stop();
 Py_RETURN_NONE;
}

static PyObject * PyTimer_getelapsed(PyTimer * self, void * closure)
{
 return PyTimer_Elapsed(self);
}

//
//
//
static PyGetSetDef PyTimer_getsetters[] = {
    { (char *)("elapsed"), (getter)PyTimer_getelapsed, NULL, (char *)("Elapsed time in seconds."), NULL},
    {NULL}  /* Sentinel */
};
 
//
//
//
static PyMethodDef PyTimer_methods[] = {
    { "Elapsed", (PyCFunction)PyTimer_Elapsed, METH_NOARGS, "Elapsed time in seconds." },
    { "Start", (PyCFunction)PyTimer_Start, METH_NOARGS, "Starts the timer." },
    { "Stop", (PyCFunction)PyTimer_Stop, METH_NOARGS, "Stops the timer." },
    {NULL}  /* Sentinel */
};

/*
 *
 */
PyTypeObject PyTimerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.Timer",                   /* tp_name */
    sizeof(PyTimer),                /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PyTimer_dealloc,    /* tp_dealloc */
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
    "Timer objects",                /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PyTimer_methods,                /* tp_methods */
    0,                              /* tp_members */
    PyTimer_getsetters,             /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PyTimer_init,         /* tp_init */
    0,                              /* tp_alloc */
    PyTimer_new,                    /* tp_new */
};

