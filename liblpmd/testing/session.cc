//
//
//

#include <lpmd/session.h>
#include "config.h"

using namespace lpmd;

Session lpmd::GlobalSession;

class lpmd::SessionImpl
{
 public: 
};

Session::Session(): Module("session") 
{
 simpl = NULL; 
 ProcessArguments("module session"); 
 
 AssignParameter("libraryversion", VERSION);

 AssignParameter("forcefactor", "0.0096485341");  // Conversion: 1 (eV/(angstrom*amu)) = FORCEFACTOR (angstrom/(fs^2))
 AssignParameter("kboltzmann", "8.6173422E-05");  // k_B in (eV/K)
 AssignParameter("kin2ev", "103.64269");          // Conversion: 1 (amu*(angstrom/fs)^2) = KIN2EV (eV)
 AssignParameter("pressfactor", "160217.65");     // Conversion: 1 (ev/(angstrom^3)) = PRESSFACTOR (MPa)
 AssignParameter("ua3togrcm3", "1.6605");         // Conversion: 1 uma/A^3 = Ua3ToGRcm3
 AssignParameter("q2a2force", "0.13893546");      // Conversion: 1 (qe*qe/angstrom*angstrom) = Q2a2FORCE (uma * angstrom / (fs^2))
 AssignParameter("q2a2ev", "14.4004");            // Conversion: 1 (qe*qe/(angstrom)) = Q2a2EV (eV)
}

Session::~Session() { }

