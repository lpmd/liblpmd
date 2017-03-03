/*
 *
 *
 *
 */

#ifndef __HAMILTONIAN_H__
#define __HAMILTONIAN_H__

#include "function.h"

template <class T> class Hamiltonian: public RealFunction<T>
{
 public:
   virtual double operator()(const T & s) const = 0;
};

#endif

