/*
 *
 *
 *
 */

#ifndef __MINIMIZER_H__
#define __MINIMIZER_H__

#include "objfunction.h"

namespace infer
{
 class Minimizer
 {
  public:
     virtual const State & Minimize(const ObjectiveFunction & obj, const State & seed, double tolerance) = 0;
     virtual void SetBoundary(const State & b1, const State & b2) = 0;
     virtual void OnIteration(int step, double objval, const State & current) const = 0;
 };
}

#endif

