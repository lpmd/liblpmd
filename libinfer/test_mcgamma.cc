/*
 *
 *
 *
 */

#include "infer/metropolis.h"
#include "infer/gammamodel.h"

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

int main()
{
 InitRandom();
 MyMetropolis m;

 GammaModel G(4.5, 3.0);
 PositiveRealState seed(10.0);
 m.SetDelta(20.0);
 m.Simulate(G, seed, 5000000, 10000000, 100);
 std::cout << "Rejection: " << m.RejectionRate() << "\n";
 std::cout << "<x> = " << x_av.Average() << "\n";
 std::cout << "<ln x> = " << logx_av.Average() << "\n";

 return 0;
}

