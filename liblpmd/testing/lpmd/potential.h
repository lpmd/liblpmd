//
//
//

#ifndef __LPMD_POTENTIAL_H__
#define __LPMD_POTENTIAL_H__

namespace lpmd
{
 class Configuration;   // forward

class Potential
{
 public:
   Potential();
   virtual ~Potential();

   void SetValidSpecies(int s1, int s2);

   bool AppliesTo(int s1, int s2) const;

   void SetCutoff(double rc) { rcutoff = rc; }

   double GetCutoff() const { return rcutoff; }

   virtual void Initialize(Configuration & conf);

   virtual double energy(Configuration & conf) = 0;
   virtual void UpdateForces(Configuration & conf) = 0;

 private:
   int spc_sum;
   double rcutoff;

 protected:
   double energycache;
};

} // lpmd

#endif


