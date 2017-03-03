/*
 *
 *
 *
 */

#include "infer/maxent.h"
#include "infer/metropolis.h"
#include "infer/objfunction.h"
#include "infer/pso.h"
#include <cmath>
#include <mpi.h>

WindowAverage x_av(300000);
WindowAverage logx_av(300000);

class MyMetropolis: public Metropolis<PositiveRealState>
{
 public:
    bool OnProductionStep(PositiveRealState & s, long int step) override
    {  
     if (step % 500 == 0)
     {
      x_av.Add(s[0]);
      logx_av.Add(log(s[0]));
     }
     return true; 
    }
};

typedef RealFunction<PositiveRealState> RRFunction;

class X: public RRFunction
{
 public:
   double operator()(const PositiveRealState & x) const override { return x.Value(); }
};

class LogX: public RRFunction
{
 public:
   double operator()(const PositiveRealState & x) const override { return log(x.Value()); }
};

class ErrorF: public ObjectiveFunction
{
 public:
    ErrorF(double F1, double F2): ObjectiveFunction(2), F1(F1), F2(F2)
    {
     G.AddConstraint(f1, F1);
     G.AddConstraint(f2, F2);
     m.SetDelta(20.0);
    }

    double operator()(const State & s) const override
    {
     x_av.Clear();
     logx_av.Clear();
     G.SetParams(s);
     PositiveRealState seed(10.0);
     m.Simulate(G, seed, 5000000, 5000000, 100);
     return sqrt(pow(F1-x_av.Average(),2)+pow(F2-logx_av.Average(),2));
    }

 private:
   double F1, F2;
   mutable MyMetropolis m;
   X f1;
   LogX f2;
   mutable MaxEntModel<PositiveRealState> G;
};

int main(int argc, char *argv[])
{
 InitRandom();
 MPI_Init(&argc, &argv);

 ErrorF e(14.0, 2.5);
 pso::Minimizer pmin(2, 30);
 State seed({1.0, -3.0});

 pmin.Minimize(e, seed);

 return 0;
}
