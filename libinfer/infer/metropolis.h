/*
 *
 *
 *
 */

#ifndef __METROPOLIS_H__
#define __METROPOLIS_H__

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

#include "random.h"
#include "block.h"
#include "average.h"
#include "function.h"

inline double RejectionSigmoid(double R)
{
 return (1.0+0.2*(1.0/(1.0+exp(7.0*(R-0.7)))-0.5));
}

class InvalidState: public std::exception
{
 public:
   const char * what() const noexcept override { return "Metropolis considered an invalid state"; }
};

template <class T> class Metropolis
{
 public:
    Metropolis() { Reset(); ClearProperties(); delta = 1.0; }

    void Reset() { MCrej = 0; MCcount = 0; }

    double RejectionRate() { return 100.0*double(MCrej)/MCcount; }

    virtual bool OnBurnInStep(T & x, long int step) { return true; }
    
    virtual bool OnProductionStep(T & x, long int step) { return true; }

    virtual bool ValidState(const T & x) const { return true; }

    void SetDelta(double d) { delta = d; }

    bool Move(const RealFunction<T> & logprob, T & s, double & logprob_s)
    {
     while (1)
     {
      s.Mutate(delta);
      if (ValidState(s)) break;
      s.UndoMutation();
     }
     double logpnew = logprob(s);
     double Q = logpnew-logprob_s;
     if (log(Random()) < Q) { logprob_s = logpnew; return true; }
     else { s.UndoMutation(); return false; }
    }

    void Sweep(const RealFunction<T> & prob, T & s, double & probs)
    {
     for (int q=0;q<s.Size();++q)
     {
      if (!Move(prob, s, probs)) MCrej++;
      MCcount++;
     }
    }

    void ClearProperties() 
    {
     properties.clear();
     datasets.clear();
    }

    void AddProperty(const RealFunction<T> & property, Block<double> & data)
    {
     properties.push_back(&property);
     datasets.push_back(&data);
    }

    void AdaptDelta()
    {
     delta *= RejectionSigmoid(RejectionRate()/100.0);
    }

    void Simulate(const RealFunction<T> & logmodel, T & x, long int steps, long int burnin=0, long int adapt=0)
    {
     double logprobx = logmodel(x);
     Reset();
     long int n = 0;
     adapt_steps = adapt;
     while (1)
     {
      try
      { 
       Sweep(logmodel, x, logprobx);
       if ((adapt_steps > 0) && (n % adapt_steps == 0)) AdaptDelta();
       if (!OnBurnInStep(x, n)) { return ; }
       n++;
      }
      catch (const InvalidState & e) { }
      if (n >= burnin) break;
     }
     for (long int n=0;n<steps;++n)
     {
      auto it2 = datasets.begin();
      for (auto it = properties.begin();it != properties.end();it++)
      {
       RealFunction<T> & property = *(*it);
       Block<double> & data = *(*it2);
       data[n] = property(x);
       it2++;
      }
      while (1)
      {
       try 
       { 
        Sweep(logmodel, x, logprobx); 
        if (!OnProductionStep(x, n)) { return; }
        break; 
       }
       catch (const InvalidState & e) { }
      }
     }
    }
  
   double delta;

 private:
   long int MCrej;
   long int MCcount;
   long int adapt_steps;
   std::vector< RealFunction<T> * > properties;
   std::vector< Block<double> * > datasets;
};

#endif

