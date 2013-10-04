/*
 *
 *
 *
 */

#ifndef __PY_TOPOLOGY_H__
#define __PY_TOPOLOGY_H__

extern "C" PyTypeObject PyTopologyType;
extern "C" PyObject * PyInitializeCommunication(PyObject * self, PyObject * args);
extern "C" PyObject * PyFinishCommunication(PyObject * self, PyObject * args);

#include <lpmd/celltopology.h>
#include <iostream>

//
//
class PyTopology
{
 public:
   PyObject_HEAD
   lpmd::CellTopology * top;
};

#endif

