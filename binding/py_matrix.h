/*
 *
 *
 *
 */

#ifndef __PY_MATRIX_H__
#define __PY_MATRIX_H__

extern "C" PyTypeObject PyMatrixType;
extern "C" PyObject * PyMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

#include <lpmd/matrix.h>

//
//
class PyMatrix
{
 public:
   PyObject_HEAD
   lpmd::Matrix * matrix;
};

#endif

