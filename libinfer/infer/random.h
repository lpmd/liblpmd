/*
 *
 *
 *
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <cstdlib>
#include <cmath>
#include <ctime>

inline long int RandomInt(long int a, long int b)
{
 return a+(rand() % (b+1-a));
}

inline double Random() { return (rand() % 10000000)/double(10000000.0); }

inline double Random(double a, double b) { return a+Random()*(b-a); }

inline double SignRandom() { return 2.0*Random()-1.0; }

inline double GaussianRandom(double mu, double sigma)
{ 
 // Box-Muller method
 double u = Random();
 double v = Random();
 return (mu+sigma*sqrt(-2.0*log(u))*cos(2.0*3.141592653589*v));
}

inline void InitRandom() { srand(time(NULL)); }

#endif

