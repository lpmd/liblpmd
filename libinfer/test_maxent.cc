/*
 *
 *
 *
 */

#include "infer/maxent.h"
#include "infer/metropolis.h"

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
      std::cout << step << "  " << x_av.Average() << "  " << logx_av.Average() << "  " << delta << "\n";
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

int main()
{
 InitRandom();
 MyMetropolis m;
 m.SetDelta(20.0);

 PositiveRealState seed(10.0);

 const State params({ 1.0/3.0, -3.5});
 X f1;
 LogX f2;

 MaxEntModel<PositiveRealState> G;
 G.AddConstraint(f1);
 G.AddConstraint(f2);
 G.SetParams(params);

 m.Simulate(G, seed, 5000000, 10000000, 100);
 std::cout << "Rejection: " << m.RejectionRate() << "\n";
 std::cout << "<x> = " << x_av.Average() << "\n";
 std::cout << "<ln x> = " << logx_av.Average() << "\n";

 return 0;
}
