//
//
//

#ifndef __LPMD_PARTICLESET_H__
#define __LPMD_PARTICLESET_H__

#include <lpmd/atom.h>

#include <vector>

namespace lpmd
{

 class ParticleSet: public std::vector<Atom>
 {
  public:
   ParticleSet(): std::vector<Atom>() { }

   ParticleSet(long int s, const Atom & at): std::vector<Atom>(s, at) { }

   Atom & Create(Atom * at) { push_back(*at); return (*at); }




 };

} // lpmd

#endif

