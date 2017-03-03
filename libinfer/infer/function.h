/*
 *
 *
 *
 */

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

template <class T, class R> class Function
{
 public:
    virtual R operator()(const T & s) const = 0;
};

template <class T> class RealFunction: public Function<T, double>
{
 public:
    virtual double operator()(const T & s) const = 0;
};

#endif

