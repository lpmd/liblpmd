/*
 *
 *
 *
 */

#ifndef __GAMMA_MODEL_H__
#define __GAMMA_MODEL_H__

#include "state.h"
#include "function.h"

inline double GammaPDF(double x, double k, double theta)
{
 return exp(-x/theta)*pow(x, k-1.0)/(gamma(k)*pow(theta,k));
}

class GammaModel: public RealFunction<PositiveRealState>
{
 public:
   GammaModel(double k, double theta): k(k), theta(theta) { }

   double operator()(const PositiveRealState & s) const override
   {
    const double & x = s[0];
    return (-x/theta + (k-1)*log(x));
   }

 private:
   double k, theta;
};

#endif

