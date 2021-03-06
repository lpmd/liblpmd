/*
 *
 *
 *
 */

#ifndef __LPMD_SIMULATION_HISTORY_H__
#define __LPMD_SIMULATION_HISTORY_H__

#include <lpmd/configurationset.h>
#include <lpmd/array.h>

namespace lpmd
{
 class SimulationHistory: public Array<Configuration&>, public ConfigurationSet
 {
  public:
    SimulationHistory();

    ~SimulationHistory();

    inline Configuration & operator[](long int i) { return Array<Configuration&>::operator[](i); }

    inline const Configuration & operator[](long int i) const { return Array<Configuration&>::operator[](i); }

    inline long int Size() const { return Array<Configuration&>::Size(); }
  
    inline long int Size() { return Array<Configuration&>::Size(); }

    void Append(Configuration & x);

    void Append(const Configuration & x);

  private:
    Array<Configuration *> configs;    
 };
}  // lpmd

#endif

