/*
 *
 *
 *
 */

#include "infer/pso.h"
#include "infer/random.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <mpi.h>

using namespace pso;

double sqr(double x) { return x*x; }

class HyperEllipsoid: public ObjectiveFunction
{
 public:
   HyperEllipsoid(unsigned long int N): ObjectiveFunction(N) { }

   double operator()(const State & p) const
   {
    double f = 0.0;
    long int N = Size();
    for (int k=1;k<=N;++k) f += pow(k*p[k-1], 2.0);
    return f;
   }
};

class Rastrigin: public ObjectiveFunction
{
 public:
   Rastrigin(unsigned long int N): ObjectiveFunction(N) { }

   double operator()(const State & p) const
   {
    double f = 100.0;
    long int N = Size();
    for (int k=1;k<=N;++k) f += (pow(p[k-1],2.0)-10.0*cos(2.0*3.14159*p[k-1]));
    return f;
   }
};

class MyMinimizer: public Minimizer
{
 public:
  MyMinimizer(int nparams, int nparticles): Minimizer(nparams, nparticles) { }

  void OnIteration(int n, double objval, const State & current) const
  { 


  }
};

int main(int argc, char *argv[])
{
 MPI_Init(&argc, &argv);
 Rastrigin f(10);
 MyMinimizer pso(10, 100);

 State state(10);
 for (int k=0;k<10;++k) state[k] = SignRandom()*100;
 const State & minst = pso.Minimize(f, state, 1.0E-20);
 for (int k=0;k<10;++k) assert (fabs(state[k]) < 100.001);
 std::cout << f(minst) << "\n";

 return 0;
}

