/*
 *
 *
 *
 */

#include <lpmd/simulationhistory.h>
#include <lpmd/storedconfiguration.h>

using namespace lpmd;

SimulationHistory::SimulationHistory() 
{ 
 configs.Clear();
}

SimulationHistory::~SimulationHistory() 
{ 
 for (int i=0;i<configs.Size();++i) delete configs[i];
}

void SimulationHistory::Append(Configuration & x)
{
 StoredConfiguration * stc = new StoredConfiguration(x); 
 configs.Append(stc);
 Array<Configuration&>::Append(*stc);
}


void SimulationHistory::Append(const Configuration & x)
{
 StoredConfiguration * stc = new StoredConfiguration(x); 
 configs.Append(stc);
 Array<Configuration&>::Append(*stc);
}

