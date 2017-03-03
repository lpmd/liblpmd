/*
 *
 *
 *
 */

#ifndef __MAXENT_H__
#define __MAXENT_H__

#include "bayes.h"
#include "state.h"
#include <list>

template <class T> class MaxEntLikelihood: public RealFunction<T>
{
 public:
    void AddConstraint(const RealFunction<T> & R) { f.push_back(&R); }

    void SetParams(const State & p) { params = p; }

    double operator()(const T & x) const override
    {
     double logL = 0.0;
     int i = 0;
     for (auto it=f.begin();it!=f.end();++it) { logL -= params[i++]*(*(*it))(x); }
     return logL;
    }

   std::list<const RealFunction<T> *> f;
   State params;
};

template <class T> class MaxEntModel: public BayesModel<T>
{
 public:
    MaxEntModel() { BayesModel<T>::SetLikelihood(L); }

    void AddConstraint(const RealFunction<T> & R) { L.AddConstraint(R); }

    void AddConstraint(const RealFunction<T> & R, double Rval)
    {
     AddConstraint(R);
     F.push_back(Rval);
    }

    void SetParams(const State & p) { L.params = p; }

    const State & Params() const { return L.params; }

 private:
   std::list<double> F;
   MaxEntLikelihood<T> L;
};

#endif

