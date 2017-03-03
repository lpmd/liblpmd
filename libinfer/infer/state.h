/*
 *
 *
 *
 */

#ifndef __STATE_H__
#define __STATE_H__

#include <iostream>
#include <cassert>
#include <initializer_list>
#include "random.h"
#include "region.h"

//
//
//
class State: public Confined
{
 public:
   State() { buffer = NULL; N = 0; }

   State(const std::initializer_list<double> & L)
   {
    N = L.size();
    Initialize(N);
    int i = 0;
    for (auto it=L.begin();it!=L.end();++it) buffer[i++] = *it;
   }
 
   State(int N): N(N)
   {
    Initialize(N);
    for (int i=0;i<N;++i) buffer[i] = 0.0;
   }

   State(const State & s)
   {
    N = s.Size();
    Initialize(N);
    for (int i=0;i<N;++i) buffer[i] = s[i];
   }

   void Initialize(int N)
   {
    this->N = N;
    buffer = new double[N];
   }

   virtual ~State() { delete [] buffer; }
   
   bool Outside(const State & b1, const State & b2) const override
   {
    assert ((b1.Size() == b2.Size()) && (Size() == b1.Size()));
    for (int q=0;q<b1.Size();++q)
    {
     if (((*this)[q] > b2[q]) || ((*this)[q] < b1[q])) return true;
    }
    return false;
   }

   const State & operator=(const State & s)
   {
    if (&s != this)
    {
     delete [] buffer;
     N = s.Size();
     Initialize(N);
     for (int i=0;i<N;++i) buffer[i] = s[i];
    }
    return s;
   }

   State operator*(double a) const
   {
    State s = (*this);
    for (int i=0;i<N;++i) s[i] *= a;
    return s;
   } 

   State operator+(const State & a) const
   {
    State s = (*this);
    for (int i=0;i<N;++i) s[i] += a[i];
    return s;
   } 

   const State & operator+=(const State & a)
   {
    for (int i=0;i<N;++i) buffer[i] += a[i];
    return (*this);
   } 

   State operator-(const State & a) const
   {
    State s = (*this);
    for (int i=0;i<N;++i) s[i] -= a[i];
    return s;
   } 

   double operator[](unsigned long int i) const { return buffer[i]; }

   double & operator[](unsigned long int i) { return buffer[i]; }

   virtual int Size() const { return N; }

   virtual void Mutate(double delta) { assert (false); }

   virtual void UndoMutation() { assert (false); }

  private:
    int N;
    double * buffer;
 };

class MutableState: public State
{
 public:
   MutableState(const std::initializer_list<double> & L): State(L) { }

   void Mutate(double delta)
   {
    int k = RandomInt(0, Size()-1);
    kbackup = k;
    backup = (*this)[k];
    (*this)[k] += SignRandom()*delta;
   }

   void UndoMutation()
   {
    (*this)[kbackup] = backup;
   }

 private:
   double backup;
   int kbackup;
};

inline std::ostream & operator<<(std::ostream & os, const State & s)
{
 os << "<";
 for (int q=0;q<s.Size();++q) os << s[q] << ", ";
 os << ">";
 return os;
}

//
//
//
class PositiveRealState: public State
{
 public:
   PositiveRealState(double x): State(1) { (*this)[0] = xold = x; }

   void Mutate(double delta) override
   { 
    xold = (*this)[0];
    while (1)
    {
     (*this)[0] = xold + SignRandom()*delta;
     if ((*this)[0] >= 0.0) break;
    }
   }

   double Value() const { return (*this)[0]; }

   void UndoMutation() override { (*this)[0] = xold; }

 private:
  double xold;
};

#endif

