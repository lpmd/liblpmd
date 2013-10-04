/*
 *
 *
 *
 */

#include <Python.h>
#include <structmember.h>
#include <iostream>
#include "py_slothandler.h"

#include <dlfcn.h>

//
// Allocator
//
extern "C" PyObject * PySlotHandler_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
 PySlotHandler * self;
 self = (PySlotHandler *)type->tp_alloc(type, 0);
 if (self != NULL) self->handle = NULL;
 return (PyObject *)self;
}

//
// Deallocator
//
extern "C" void PySlotHandler_dealloc(PySlotHandler * self)
{
 if (self->handle != NULL) dlclose(self->handle);
 Py_TYPE(self)->tp_free((PyObject*)self);
}

//
//  __init__
//
extern "C" int PySlotHandler_init(PySlotHandler * self, PyObject *args, PyObject *kwds)
{
 PyObject * path_str = NULL;
 static char * kwlist[] = { (char *)("path"), NULL};
 if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &path_str)) return -1;
 PyObject * pystr = PyUnicode_AsEncodedString(path_str, "ascii", "Error ~");
 Py_INCREF(pystr);
 const char * str = PyBytes_AS_STRING(pystr);
 self->handle = dlopen(str, RTLD_LAZY);
 if (self->handle == NULL)
 {
  PyErr_SetString(PyExc_Exception, str);
  return -1;
 }
 else return 0;
}

PyObject * PySlotHandler_GetSlot(PySlotHandler * self, PyObject * args)
{
 PyObject * name_str = NULL;
 PyArg_ParseTuple(args, "O", &name_str); 
 PyObject * pystr = PyUnicode_AsEncodedString(name_str, "ascii", "Error ~");
 Py_INCREF(pystr);
 const char * name = PyBytes_AS_STRING(pystr);
 void * ptr = dlsym(self->handle, name);
 if (ptr != NULL)
 {
  char ptrstr[50];
  snprintf(ptrstr, 50, "%p", ptr);
  PyObject * ptrobj = Py_BuildValue("s", ptrstr);
  Py_INCREF(ptrobj);
  return ptrobj;
 }
 else
 {
  PyErr_SetString(PyExc_AttributeError, name);
  return NULL;
 }
}

PyObject * PySlotHandler_GetMetadata(PySlotHandler * self)
{
 const char * (*pmeta)();
 pmeta = ( const char * (*)() )(dlsym(self->handle, "Plugin_Meta"));
 if (!pmeta)
 {
  PyErr_SetString(PyExc_Exception, "Plugin has no metadata.");
  return NULL;
 }
 assert (pmeta);
 const char * metadata_pickled = (*pmeta)();
 PyObject * ptrobj = Py_BuildValue("s", metadata_pickled);
 Py_INCREF(ptrobj);
 return ptrobj;
}

//
//
//
static PyMethodDef PySlotHandler_methods[] = {
    { "GetSlot", (PyCFunction)PySlotHandler_GetSlot, METH_VARARGS, "Retrieves the named slot of the Plugin." },
    { "GetMetadata", (PyCFunction)PySlotHandler_GetMetadata, METH_NOARGS, "Retrieves the plugin\'s metadata." },
    {NULL}  /* Sentinel */
};

/*
 *
 */
PyTypeObject PySlotHandlerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "LPMD.SlotHandler",             /* tp_name */
    sizeof(PySlotHandler),          /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)PySlotHandler_dealloc,   /* tp_dealloc */
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
    "SlotHandler objects",          /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    PySlotHandler_methods,          /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)PySlotHandler_init,   /* tp_init */
    0,                              /* tp_alloc */
    PySlotHandler_new,              /* tp_new */
};

