//
//
//

/** 
 *  \file module.h
 *  \brief Declaración de la clase Module y las excepciones asociadas a Module.
 *  \author GNM
 */

#ifndef __LPMD_MODULE_H__
#define __LPMD_MODULE_H__

#include <lpmd/paramlist.h>
#include <lpmd/error.h>

#include <list>

namespace lpmd
{

/**
 *  Module es la clase base de la cual deben derivar todos los módulos que LPMD usa como plugins.
 */
class Module: public ParamList
{
 public:
   /// Constructor por omisión.
   Module();
   /// Constructor que recibe un nombre para el módulo.
   Module(std::string modulename);
   /// Constructor de copia
   Module(const Module & mod);
   /// Destructor.
   virtual ~Module();

   void ProcessArguments(std::string line);
   std::string GetNextWord();
   std::string GetNextWord(char);

   virtual void ShowHelp() const;

   std::string Name() const;
   bool Used() const;
   void SetUsed();

   // Operador de asignacion
   Module & operator=(const Module & mod);

   // Virtuales con implementacion por omisión
   virtual void Show() const; // This defaults to Show(std::cout) but it depends on the implementation of liblpmd
   virtual void Show(std::ostream & os) const; 
   virtual std::string Provides() const;
   virtual double GetProperty(const std::string & name);
   virtual void SetParameter(std::string name);

   // Virtuales puros
   virtual std::string Keywords() const = 0;

   void (*unloader)(Module *);

 private:
   class ModuleImpl * impl;
};

//
class InvalidModuleType: public Error
{
 public:
   InvalidModuleType();
};

class UnknownProperty: public Error
{
 public:
   UnknownProperty(const std::string name);
};

} // lpmd

#endif


