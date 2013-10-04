/*
 *
 *
 *
 */

#include <Python.h>
#include "py_cell.h"
#include "py_topology.h"
#include "py_atomset.h"
#include "py_neighborlist.h"
#include "py_slothandler.h"
#include "py_slot.h"
#include "py_timer.h"
#include "py_matrix.h"
#include "py_atomselection.h"

/*
 *
 * Module-level global functions
 *
 */

static PyMethodDef lpmdfunctions[] = {
    {"InitializeCommunication",  PyInitializeCommunication, METH_VARARGS, "Initializes MPI."},
    {"FinishCommunication",  PyFinishCommunication, METH_VARARGS, "Finishes MPI."},
    {NULL},        /* Sentinel */
};

static PyModuleDef lpmdmodule = {
    PyModuleDef_HEAD_INIT,
    "lpmd",
    "Las Palmeras Molecular Dynamics module.",
    -1,
    lpmdfunctions
};

extern "C" PyMODINIT_FUNC PyInit_lpmdmodule(void)
{
 PyObject * m;
 if (PyType_Ready(&PyCellType) < 0) return NULL;
 if (PyType_Ready(&PyTopologyType) < 0) return NULL;
 if (PyType_Ready(&PyAtomSetType) < 0) return NULL;
 if (PyType_Ready(&PyNeighborListType) < 0) return NULL;
 if (PyType_Ready(&PySlotHandlerType) < 0) return NULL;
 if (PyType_Ready(&PySlotType) < 0) return NULL;
 if (PyType_Ready(&PyTimerType) < 0) return NULL;
 if (PyType_Ready(&PyMatrixType) < 0) return NULL;
 if (PyType_Ready(&PyAtomSelectionType) < 0) return NULL;
 m = PyModule_Create(&lpmdmodule);
 if (m == NULL) return NULL;
 Py_INCREF(&PyCellType);
 Py_INCREF(&PyTopologyType);
 Py_INCREF(&PyAtomSetType);
 Py_INCREF(&PyNeighborListType);
 Py_INCREF(&PySlotHandlerType);
 Py_INCREF(&PySlotType);
 Py_INCREF(&PyTimerType);
 Py_INCREF(&PyMatrixType);
 Py_INCREF(&PyAtomSelectionType);
 PyModule_AddObject(m, "Cell", (PyObject *)&PyCellType);
 PyModule_AddObject(m, "Topology", (PyObject *)&PyTopologyType);
 PyModule_AddObject(m, "AtomSet", (PyObject *)&PyAtomSetType);
 PyModule_AddObject(m, "NeighborList", (PyObject *)&PyNeighborListType);
 PyModule_AddObject(m, "SlotHandler", (PyObject *)&PySlotHandlerType);
 PyModule_AddObject(m, "Slot", (PyObject *)&PySlotType);
 PyModule_AddObject(m, "Matrix", (PyObject *)&PyMatrixType);
 PyModule_AddObject(m, "AtomSelection", (PyObject *)&PyAtomSelectionType);
 PyModule_AddObject(m, "Timer", (PyObject *)&PyTimerType);
 return m;
}

