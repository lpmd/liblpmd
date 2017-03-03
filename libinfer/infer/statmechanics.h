/*
 *
 *
 *
 */

#ifndef __STATMECHANICS_H__
#define __STATMECHANICS_H__

#include "maxent.h"
#include "hamiltonian.h"

template <class T> class CanonicalModel: public MaxEntModel<T>
{
 public:
   CanonicalModel(const Hamiltonian<T> & H, double beta)
   {
    MaxEntModel<T>::SetParams(State({beta}));
    MaxEntModel<T>::AddConstraint(H);
   }
};

inline double Heaviside(double x)
{
 if (x < 0.0) return 1.0E-80;
 else return 1.0;
}

template <class T> class QExpHeaviside: public RealFunction<T>
{
 public:
    QExpHeaviside(const Hamiltonian<T> & H, double beta, double q): H(H), beta(beta), q(q) { }

    double operator()(const T & x) const override { return log(Heaviside(1.0-(1.0-q)*beta*H(x))); }

 private:
    const Hamiltonian<T> & H;
    double beta, q;
};

template <class T> class QExpLogP: public RealFunction<T>
{
 public:
    QExpLogP(const Hamiltonian<T> & H, double beta, double q): H(H), beta(beta), q(q) { }

    double operator()(const T & x) const override { return (1.0/(q-1.0))*log(1.0-(1.0-q)*beta*H(x)); }

 private:
    const Hamiltonian<T> & H;
    double beta, q;
};

template <class T> class QCanonicalModel: public MaxEntModel<T>
{
 public:
   QCanonicalModel(const Hamiltonian<T> & H, double beta, double q)
   {
    qexp_heaviside = new QExpHeaviside<T>(H, beta, q);
    qexp_logp = new QExpLogP<T>(H, beta, q);
    MaxEntModel<T>::SetPrior(qexp_heaviside);
    MaxEntModel<T>::AddConstraint(qexp_logp);
   }

   virtual ~QCanonicalModel()
   {
    delete qexp_heaviside;
    delete qexp_logp;
   }

 private:
   RealFunction<T> * qexp_heaviside;
   RealFunction<T> * qexp_logp;
};

#endif

