//
//
//

/**
 *  \file particleset.h
 *  \brief Declaración de la clase ParticleSet.
 *  \author GNM
 */

#ifndef __LPMD_PARTICLESET_H__
#define __LPMD_PARTICLESET_H__

#include <vector>
#include <lpmd/atom.h>

namespace lpmd
{

 /*
  *
  */
 class ParticleSet: public std::vector<Atom *>
 {
  public:
    ParticleSet();                       // Constructor por omision
    ParticleSet(long int s);             // Constructor con numero de atomos
    ParticleSet(const ParticleSet & ps); // Constructor de copia
    
    virtual ~ParticleSet();

    // Agrega un atomo ya existente
    inline Atom & Add(Atom & at) { push_back(&at); return at; }

    Atom & Create(Atom * at);     // Crea un nuevo atomo en el espacio de memoria de ParticleSet

    // Elimina una referencia a un atomo existente
    inline void Remove(Atom & at) 
    { 
     for (std::vector<Atom *>::iterator it=begin();it!=end();++it) 
         if ((*it) == &at) erase(it);
    }

    void Destroy(Atom * at);    // Elimina definitivamente un atomo del espacio de memoria de ParticleSet

    // Emula el acceso a ParticleSet como si fuera un vector de referencias  
    inline Atom & operator[](size_type i) { return *(std::vector<Atom *>::operator[](i)); }
    inline const Atom & operator[](size_type i) const { return *(std::vector<Atom *>::operator[](i)); }

    // FIXME: Por ahora, para compatibilidad con Array y todo lo otro
    inline long int Size() const { return size(); }

    // Operador de asignacion
    ParticleSet & operator=(const ParticleSet & ps);
  
    void WriteAll(std::ostream & out);
 
    void NumEspec();
    void AssignIndex();

    std::list<std::string> SpeciesList() const;
    std::list<std::string> SpeciesTriplets() const;
    std::list<std::string> SpeciesPairs() const;
    std::list<std::string> RepeatedSpeciesPairs() const;
    std::vector<int> NSpecies() const;

    void InitVelocities();
    void SetTemperature(double temp, double dt=1.0, double tau=1.0);
    double KineticEnergy() const;
    double Temperature() const;
    double Momentum() const;
 
  private:
    std::vector<Atom *> managed;
    int nspec;
    int * spec;
 };

} // lpmd

#endif

