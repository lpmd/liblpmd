/*
 * Ejemplo simple de dinamica molecular usando el API de liblpmd
 */

#include <lpmd/pluginmanager.h>
#include <lpmd/timer.h>
#include <lpmd/simulation.h>

#include <iostream>

using namespace lpmd;

int main()
{
 Simulation md;                                            // define md como un objeto de dinamica molecular
 PluginManager pm;                                 // define pm como un manejador de plugins

 // Carga de plugins con sus parametros
 pm.LoadPlugin("test", "");

 pm["test"].PerformTest(md);

 return 0;
}

