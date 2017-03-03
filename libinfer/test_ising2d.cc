/*
 *
 *
 *
 */

#include "infer/metropolis.h"
#include "infer/ising2d.h"
#include "infer/statmechanics.h"

inline int BinaryToSpin(int v) { return 2*v-1; } 

class IsingHamiltonian: public Hamiltonian<Ising2DState>
{
 public:
   IsingHamiltonian(double J): J(J) { }

   double operator()(const Ising2DState & s) const override
   {
    int N = s.LinearSize();
    double E = 0.0;
    for (int i=0;i<N;++i)
        for (int j=0;j<N;++j) 
        {
         int i_up = ( (i == N-1) ? 0 : (i+1));
         int j_up = ( (j == N-1) ? 0 : (j+1));
         E += BinaryToSpin(s.Get(i, j))*(BinaryToSpin(s.Get(i_up, j))+BinaryToSpin(s.Get(i, j_up)));
        }
    return -J*E;
   }

 private:
   double J;
};

class Magnetization: public RealFunction<Ising2DState>
{
 public:
   double operator()(const Ising2DState & s) const override
   {
    int N = s.LinearSize();
    double M = 0.0;
    for (int i=0;i<N;++i)
        for (int j=0;j<N;++j) 
        {
         M += BinaryToSpin(s.Get(i, j));
        }
    return M/float(N*N);
   }
}; 

WindowAverage energy(300000);
WindowAverage magnet(300000);
IsingHamiltonian H(1.0);
Magnetization MM;

class MyMetropolis: public Metropolis<Ising2DState>
{
 public:
    bool OnProductionStep(Ising2DState & s, long int step) override
    {  
     if (step % 100 == 0) 
     {
      energy.Add(H(s));
      magnet.Add(MM(s));
      std::cout << step << "  " << magnet.Average() << "\n";
     }
     return true; 
    }
};

int main()
{
 InitRandom();
 MyMetropolis m;

 int L = 32;
 Ising2DState seed(L);
 double T = 2.2;
 for (int i=0;i<L;++i) 
    for (int j=0;j<L;++j) seed.Set(i, j, 1);
 CanonicalModel<Ising2DState> canonical(H, 1.0/T);
 m.Simulate(canonical, seed, 50000, 50000);
 std::cout << T << "  " << energy.Average()/float(L*L) << "  " << "  " << magnet.Average() << "  " <<  m.RejectionRate() << "\n";

 return 0;
}

