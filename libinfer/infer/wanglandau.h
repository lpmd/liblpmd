/*
 *
 *
 *
 */

#ifndef __WANGLANDAU_H__
#define __WANGLANDAU_H__

#include <cmath>
#include "metropolis.h"

template <class T> class InverseOmega: public RealFunction<T>
{
 public:
   InverseOmega(const RealFunction<T> & h, double emin, double emax, int bins): H(h), emin(emin), emax(emax), bins(bins)
   {
    log_omega = new double[bins];
    Clear();
   }

   virtual ~InverseOmega() { delete [] log_omega; }

   void Clear()
   {
    for (int i=0;i<bins;i++) log_omega[i] = 0.0;
   }
 
   double operator()(const T & x) const 
   {
    long int k = (long int)(bins*((H(x)-emin)/(emax-emin)));
    return -log_omega[k];
   }

   void MarkVisited(double E, double f)
   {
    long int k = (long int)(bins*((E-emin)/(emax-emin)));
    log_omega[k] += log(f);
   }

   double LogOmega(double E) const
   {
    long int k = (long int)(bins*((E-emin)/(emax-emin)));
    return log_omega[k];
   }

 private:
   const RealFunction<T> & H;
   double emin, emax;
   int bins;
   double * log_omega;
};

template <class T> class WangLandau: public Metropolis<T>
{
 public:
    WangLandau(const RealFunction<T> & energy, double emin, double emax, int bins): energy(energy), emin(emin), emax(emax), bins(bins)
    {
     logp = new InverseOmega<T>(energy, emin, emax, bins);
     histogram = new unsigned long int[bins];
    }

    virtual ~WangLandau()
    {
     delete [] histogram;
     delete logp;
    }

    bool ValidState(const T & x) const override
    {
     double E = Energy()(x);
     if ((E > emax) || (E < emin)) return false;
     return true;
    }

    bool OnProductionStep(T & x, long int step) override
    {
     double E = energy(x);
     logp->MarkVisited(E, f);
     long int k = (long int)(bins*((E-emin)/(emax-emin)));
     histogram[k]++;
     return OnStep(x, step);
    }

    virtual bool OnStep(T & x, long int step) { return true; }

    void Simulate(T & seed, double f)
    {
     double E = energy(seed);
     this->f = f;
     assert ((E >= emin) && (E <= emax));
     ClearHistogram();
     while (1)
     {
      Metropolis<T>::Simulate(*logp, seed, 100000, 0, 0);
      double F = Flatness();
      int cnt = 0;
      for (int i=0;i<bins;i++) 
          if (histogram[i] == 0) cnt++;
      if (F > 95.0) break;
     }
    }

    void ClearHistogram()
    {
     for (int i=0;i<bins;i++) histogram[i] = 0;
    }

    double LogOmega(double E) const
    {
     return logp->LogOmega(E);
    }

    unsigned long int Histogram(double E) const
    {
     long int k = (long int)(bins*((E-emin)/(emax-emin)));
     return histogram[k];
    }

    double HistogramEntropy() const
    {
     double sum = 0.0, S = 0.0;
     for (int i=0;i<bins;++i) sum += histogram[i];
     for (int i=0;i<bins;++i)
     {
      if (histogram[i] == 0) continue;
      const double p = float(histogram[i])/sum;
      S -= p*log(p);
     }
     return S-log(bins);
    }

    double Flatness() const
    {
     double sum = 0.0, hmin = histogram[0];
     for (int i=0;i<bins;++i) 
     {
      sum += histogram[i];
      if (histogram[i] < hmin) hmin = histogram[i];
     }
     return (100.0*hmin/(float(sum)/bins));
    }

    const RealFunction<T> & Energy() const { return energy; }
 
 private:
    const RealFunction<T> & energy;
    double emin, emax;
    int bins;
    InverseOmega<T> * logp;
    unsigned long int * histogram;
    double f;
};

#endif

