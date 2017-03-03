/*
 *
 *
 *
 */

#ifndef __OBJFUNCTION_H__
#define __OBJFUNCTION_H__

#include "function.h"
#include "state.h"

//
//
//
class ObjectiveFunction: public RealFunction<State>
{
 public:
   ObjectiveFunction(unsigned long int N): N(N) { }

   inline unsigned long int Size() const { return N; }

   virtual ~ObjectiveFunction() { }

   virtual double operator()(const State & p) const = 0;

  private:
    unsigned long int N;
};

#endif

