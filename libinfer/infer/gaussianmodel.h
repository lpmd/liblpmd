/*
 *
 *
 *
 */

#ifndef __GAUSSIAN_MODEL_H__
#define __GAUSSIAN_MODEL_H__

#include "state.h"
#include "function.h"

inline double GaussianPDF(double x, double mu, double sigma)
{
 return exp(-(0.5/(sigma*sigma)))*pow(x-mu,2.0)/(sqrt(2.0*3.141592653589)*sigma);
}

class GaussianModel: public RealFunction<PositiveRealState>
{
 public:
   GaussianModel(double mu, double sigma): mu(mu), sigma(sigma) { }

   double operator()(const PositiveRealState & s) const override
   {
    const double & x = s[0];
    return (-(0.5/(sigma*sigma)))*pow(x-mu,2.0);
   }

 private:
   double mu, sigma;
};

#endif

