#ifndef __BEZIER_H__
#define __BEZIER_H__

#include "state.h"
#include <cmath>
#include <vector>

inline int factorial(int n)
{
 if (n==0) { return 1; }
 else { return n*factorial(n-1); }
}

inline double b(int i, int N, double t)
{
 return double(factorial(N))/double(factorial(N-i)*factorial(i))*pow(t,i)*pow(1.0-t,N-i);
}

inline double dotb(int i, int N, double t)
{
 return double(factorial(N))/double(factorial(N-i)*factorial(i))*(i*pow(t,i-1)*pow(1.0-t,N-i) - pow(t,i)*(N-i)*pow(1.0-t,N-i-1));
}

inline double ddotb(int i, int N, double t)
{
 return double(factorial(N))/double(factorial(N-i)*factorial(i))*(i*(i-1)*pow(t,i-2)*pow(1.0-t,N-i) - (N-i)*i*pow(t,i-1)*pow(1.0-t,N-i-1) - i*pow(t,i-1)*(N-i)*pow(1.0-t,N-i-1) + pow(t,i)*(N-i)*(N-i-1)*pow(1.0-t,N-i-2));
}

class Bezier 
{
 public:
    Bezier(const std::vector<State> & S): ctrpoints(S) { ncontrol = S.size(); }
   
	Bezier(const std::initializer_list<State> & S)
    {
	 ncontrol = 0.0;
	 for (auto it=S.begin();it!=S.end();++it)
     {
	  ncontrol +=1;
	  ctrpoints.push_back(*it);
     }
    }

    Bezier(int N)
    {
     for (int i=0;i<N;++i) ctrpoints.push_back(State());
     ncontrol = N;
    }

    virtual State operator()(double t) const
    {
	 State s(ctrpoints[0].Size());
	 for (unsigned int j=0; j < ctrpoints.size(); j++)
     {
	  for (int i=0; i <= ctrpoints[j].Size(); i++)
      {
	   s[i] += ctrpoints[j][i]*b(j,ncontrol-1,t);
      }
     }
     return s;
    }

	State Dot(double t) const 
    { 
	 State s(ctrpoints[0].Size());
	 for (unsigned int j=0; j < ctrpoints.size(); j++)
     {
	  for (int i=0; i <= ctrpoints[j].Size(); i++)
      {
	   s[i] += ctrpoints[j][i]*dotb(j,ncontrol-1,t);
      }
     }
	 return s;
    }

	State Ddot(double t) const 
    { 
	 State s(ctrpoints[0].Size());
	 for (unsigned int j=0; j < ctrpoints.size(); j++)
     {
	  for (int i=0; i <= ctrpoints[j].Size(); i++)
      {
	   s[i] += ctrpoints[j][i]*ddotb(j,ncontrol-1,t);
      }
     }
	 return s;
    }

	int Size() const { return ctrpoints[0].Size(); }

	void Mutate(double delta)
    {
	 if(ctrpoints.size() > 2)
     {
	  i = 1 + random()%(ctrpoints.size()-2);
	  j = random()%ctrpoints[i].Size();
     }
	 else
     {
	  i = random()%ctrpoints.size();
	  j = random()%ctrpoints[i].Size();
     }
	 exctrpoint = ctrpoints[i][j];
	 ctrpoints[i][j] += SignRandom()*delta;
    }

	void UndoMutation() { ctrpoints[i][j] = exctrpoint; }

    const std::vector <State> & ControlPoints() const { return ctrpoints; }
    std::vector <State> & ControlPoints() { return ctrpoints; }

private:
   std::vector <State> ctrpoints;
   int ncontrol; 
   double exctrpoint; 
   int i; int j;
};

#endif
