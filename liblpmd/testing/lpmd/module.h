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

#include <list>
#include <iostream>

namespace lpmd
{
 class PluginManager;        // forward declaration

/**
 *  Module es la clase base de la cual deben derivar todos los módulos que LPMD usa como plugins.
 */
class Module: public ParamList
{
 public:
   /// Constructor que recibe un nombre para el módulo.
   Module(std::string modulename, bool strictkw=true);
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

   PluginManager & GetManager() const;

   // 
   void AssignParameter(const std::string & key, std::string value);
   void DefineKeyword(const std::string kw, const std::string defvalue);
   void DefineKeyword(const std::string kw);

   // Flujo de depuracion 
   void SetDebugFile(const std::string debugfile);
   void SetDebugStream(std::ostream & ostr);
   std::ostream & DebugStream() const;

   // Operador de asignacion
   Module & operator=(const Module & mod);

   // Virtuales con implementacion por omisión
   virtual void Show() const;        // This defaults to Show(std::cout) but it depends on the implementation of liblpmd
   virtual void Show(std::ostream & os) const; 
   virtual std::string Provides() const;
   virtual double GetProperty(const std::string & name);
   virtual void SetParameter(std::string name);
   virtual std::string Keywords() const;

   void (*unloader)(Module *);

   // FIXME : Cambiar este metodo por un parametro extra en el constructor
   void SetManager(PluginManager & pm);

 private:
   class ModuleImpl * impl;
};

} // lpmd

#endif


