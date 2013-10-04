/*
 *
 *
 *
 */

#ifndef __PY_SLOTHANDLER_H__
#define __PY_SLOTHANDLER_H__

extern "C" PyTypeObject PySlotHandlerType;

//
//
class PySlotHandler
{
 public:
   PyObject_HEAD
   void * handle;
};

#endif

