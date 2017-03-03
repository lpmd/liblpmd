/*
 *
 *
 *
 */

#include <iomanip>
#include "infer/hamiltonian.h"
#include "infer/wanglandau.h"
#include "infer/state.h"

class Quadratic: public Hamiltonian<MutableState>
{
 public:
    double operator()(const MutableState & s) const override
    {
     double S = 0.0;
     for (int q=0;q<s.Size();++q) S += s[q]*s[q];
     S = 0.5*S;
     return S;
    }
};

class MyWL: public WangLandau<MutableState>
{
 public:
   MyWL(const Quadratic & h, double emin, double emax, int bins): WangLandau<MutableState>(h, emin, emax, bins)
   {
    this->emin = emin;
    this->emax = emax;
   }

 private:
   double emin, emax;
};

int main()
{
 InitRandom();
 Quadratic H;
 MyWL WL(H, 0.0, 150.0, 300);
 MutableState seed({ 4.0, 4.0, 4.0, 4.0, 4.0 });
 WL.SetDelta(5.0);
 double f = exp(1.0);
 std::cerr << std::setprecision(10);
 std::cout << std::setprecision(10);
 while ((f-1.0) > 1.0E-9)
 {
  WL.Simulate(seed, f);
  f = sqrt(f);
 }
 for (int i=0;i<100;++i)
 {
  double E = float(i);
  std::cout << E << "  " << WL.LogOmega(E) << "  " << WL.Histogram(E) << "\n";
 }
 return 0;
}

