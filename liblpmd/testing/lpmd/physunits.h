//
//
//

#ifndef __LPMD_PHYSUNITS_H__
#define __LPMD_PHYSUNITS_H__

// k_B in (eV/K)
#define KBOLTZMANN 8.6173422E-05

// Conversion: 1 (amu*(angstrom/fs)^2) = KIN2EV (eV)
#define KIN2EV 103.64269 

// Conversion: 1 (eV/(angstrom*amu)) = FORCEFACTOR (angstrom/(fs^2))
#define FORCEFACTOR 0.0096485341 

// Conversion: 1 (ev/(angstrom^3)) = PRESSFACTOR (MPa)
#define PRESSFACTOR 160217.65 

// Conversion: 1 (qe*qe/(angstrom)) = Q2a2EV (eV)
#define Q2a2EV 14.4004

// Conversion: 1 (qe*qe/angstrom*angstrom) = Q2a2FORCE (uma * angstrom / (fs^2))
#define Q2a2FORCE 0.13893546

#endif

