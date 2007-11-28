//
//
//

/** 
 *  \file module.h
 *  \brief Declaraci�n de la clase Module y las excepciones asociadas a Module.
 *  \author GNM
 */

#ifndef __LPMD_MODULE_H__
#define __LPMD_MODULE_H__

#include "paramlist.h"

#include <list>
#include <exception>

namespace lpmd
{

/**
 *  Module es la clase base de la cual deben derivar todos los m�dulos que LPMD usa como plugins.
 */
class Module: public ParamList
{
 public:
   /// Constructor por omisi�n.
   Module();
   /// Constructor que recibe un nombre para el m�dulo.
   Module(std::string modulename);
   /// Destructor.
   virtual ~Module();

   void ProcessArguments(std::string line);
   std::string GetNextWord();
   std::string GetNextWord(char);

   virtual void ShowHelp() const;

   std::string Name() const;
   bool Used() const;
   void SetUsed();

   virtual void Show() const; 
   virtual void SetParameter(std::string name) = 0;
   virtual std::string Keywords() const = 0;

   void (*unloader)(Module *);

 protected:
   std::list<std::string> words;
   std::string name;

 private:
   bool used;
};

//
class InvalidModuleType: public std::exception
{
 public:
   const char * what() const throw();
};

} // lpmd

#endif


