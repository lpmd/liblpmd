/*
 *
 *
 *
 */

#ifndef __PY_SLOT_H__
#define __PY_SLOT_H__

extern "C" PyTypeObject PySlotType;

//
//
class PySlot
{
 public:
   PyObject_HEAD
   void * funcptr;
   void * setkw;
   bool is_mask, inverse;
};

#endif

