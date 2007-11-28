//
//
//

/**
 *  \file particles.h
 *  \brief Declaración de la clase Particles.
 *  \author GNM
 */

#ifndef __LPMD_PARTICLES_H__
#define __LPMD_PARTICLES_H__

#include "atom.h"
#include <list>
#include <vector>
#include <fstream>

namespace lpmd
{

/**
 *  Particles representa un conjunto de objetos Atom.
 */
class Particles
{
    public:
      Particles();
      Particles(Atom *a, int N) { spec = NULL; for (int i=0;i<N;i++) A.push_back(a[i]); }
      Particles(const Particles &a) { spec = NULL; A=a.A; }
      virtual ~Particles();

      //Settings and Gettings
      const Atom & GetAtom(int i) const { return A[i]; }
      void SetAtom(Atom a,int i){A[i]=a;}
      void SetAtomType(int i, AtomType & at) { A[i].SetType(at); }
      int Size() const {return A.size();}

      void Initialize(long atoms)
      {
       A.clear();
       A.reserve(atoms);
      }
   
      void NumEspec();             //Se debe llamar una sola vez(por ahora) para asignar el numero de especies a nspec y los tipos a spec

      //Retorno de Valores Privados.
      int NEspec() const { return nspec; }
      int* Espec() const { return spec; }
     
      std::list<std::string> SpeciesTriplets() const;
      std::list<std::string> SpeciesPairs() const;
      std::list<std::string> RepeatedSpeciesPairs() const;
      std::list<std::string> SpeciesList() const;

      const Atom & operator[](int i) const { return A[i]; }

      void InitVelocities();
      void SetTemperature(double temp, double dt=1.0, double tau=1.0);

      // Propiedades fisicas
      double KineticEnergy() const;
      double Temperature() const;
      double Momentum() const;

      // Metodos de escritura, para volcar toda la informacion en dumping.
      void WriteAll(std::ostream&);

      //Sobrecarga operador de asignacion
      inline Particles& operator= (const Particles& B)
      {
       A.clear();
	   for(unsigned int i=0;i<B.A.size();i++){A.push_back(B.A[i]);}
	   return(*this);
      }
 
      //Sobrecarga in/out
      friend std::ostream& operator<< (std::ostream&, const lpmd::Particles&);
      friend std::istream& operator>> (std::istream&, Particles&);
      
      //Utils
      void AppendAtom(Atom a) { A.push_back(a); }

   private: 
      std::vector<Atom> A;
      int nspec;
      int *spec;
};

} // lpmd

#endif

