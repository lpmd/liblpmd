/*
 *
 *
 *
 */

#ifndef SERIAL
#include <mpi.h>
#else
#include <time.h>
#endif

#include <iostream>
#include <lpmd/timer.h>

using namespace lpmd;

class lpmd::TimerImpl
{
 public:
   TimerImpl() 
   { 
#ifndef SERIAL
    t0 = MPI_Wtime();
#else
    t0 = time(NULL);
#endif
    t1 = t0; 
   }
   double t0, t1;
};

Timer::Timer() { impl = new TimerImpl(); }

Timer::~Timer() { delete impl; }

void Timer::Start() 
{ 
#ifndef SERIAL
 impl->t0 = MPI_Wtime(); 
#else
 impl->t0 = time(NULL);
#endif
 impl->t1 = impl->t0; 
}

void Timer::Stop() 
{ 
#ifndef SERIAL
 impl->t1 = MPI_Wtime(); 
#else
 impl->t1 = time(NULL);
#endif
}

double Timer::Elapsed() const { return (impl->t1-impl->t0); }

