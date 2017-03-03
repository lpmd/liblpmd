/*
 *
 *
 *
 */

#ifndef __AVERAGE_H__
#define __AVERAGE_H__

#include <cmath>
#include "block.h"

class PlusMinus
{
 public:
    double mean;
    double stdev;

    PlusMinus(const Block<double> & block)
    {
     double S = 0.0, S2 = 0.0;
     for (long int i=0;i<block.Size();++i) { S += block[i]; S2 += (block[i]*block[i]); }
     mean = S/double(block.Size());
     stdev = (sqrt(block.Size())/sqrt(block.Size()-1))*sqrt(S2/double(block.Size())-(mean*mean));
    }
};

inline double AverageOfBlock(double * data, long int n)
{
 double S = 0.0;
 for (long int i=0;i<n;++i) S += data[i];
 return S/double(n);
}

class WindowAverage
{
 public:
   WindowAverage(long int nwindow): n(nwindow) 
   { 
    Clear();
   }

   WindowAverage(): n(1)
   {
    Clear();
   }

   void Clear()
   {
    count = 0;
    xav = 0.0;
    xav2 = 0.0;
   }

   void Add(double x) 
   { 
    if (count == n) { xav += (x-last); xav2 += (x*x-last*last); }
    else { xav += x; xav2 += x*x; count++; }
    last = x; 
   }

   bool Full() const { return (count == n); }

   double Average() const { return xav/count; }

   double Stdev() const { return sqrt((xav2/count)-pow(xav/count,2.0)); }

 private:
   long int n, count;
   double xav, xav2, last;
};

class SimpleAverage
{
 public:
   SimpleAverage(long int nwindow): n(nwindow)
   {
    Clear();
   }

   SimpleAverage(): n(1)
   {
    Clear();
   }

   void Clear()
   {
    count = 0;
    xav = 0.0;
    xav2 = 0.0;
   }

   void Add(double x) 
   { 
    if (count == n)
    {
     count = 0;
     xav = 0.0;
     xav2 = 0.0;
    }
    xav += x;
    xav2 += x*x;
    count++;
   }

   bool Full() const { return (count == n); }
   
   double Average() const { return xav/count; }

   double Stdev() const { return sqrt((xav2/count)-pow(xav/count,2.0)); }

 private:
   long int n, count;
   double xav, xav2;
};

#endif

