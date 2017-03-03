/*
 *
 *
 *
 */

#ifndef __PSO_H__
#define __PSO_H__

#include "minimizer.h"

//
//
//
namespace pso
{

 class Minimizer: public infer::Minimizer
 {
  public:
    Minimizer(int params, int particles);
    virtual ~Minimizer();

    const State & Minimize(const ObjectiveFunction & obj, const State & seed, double tolerance=1.0e-10) override;
 
    double tolerance, omega, c1, c2;

    void SetBoundary(const State & b1, const State & b2) override;
 
    virtual void AdvanceParticles(const ObjectiveFunction & obj, const State & globalmin);

    virtual void OnIteration(int step, double objval, const State & current) const override { }

 private:
   int nparams, nparticles, nprocs;
   double * minibuffer, * buffer;
   State current;
   State * x, *v, *localmin;
   State b1, b2;
   bool boundary_set;
   void UpdateGlobalMinimum(const ObjectiveFunction & obj);
 };

}

#endif

