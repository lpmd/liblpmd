//
//
//

#ifndef __LPMD_INPUTREADER_H__
#define __LPMD_INPUTREADER_H__

#include <list>
#include <map>
#include "paramlist.h"

namespace lpmd
{

class InputSyntaxError: public std::exception
{
 public:
   const char * what() const throw();
};

class InputFile: public ParamList
{
 public:
   InputFile();
   virtual ~InputFile();

   void DeclareStatement(const std::string & cmd, const std::string & args);

   virtual void Read(std::string sysfile);
   virtual int OnStatement(const std::string & name, const std::string & keywords, bool regular);

 protected:
   std::list<std::string> words; // FIXME: hacer que words sea private, implementar funciones de acceso
   std::string MatchCommand(std::list<std::string> & w);
   std::string ParseCommandArguments(const std::string & cmd, const std::string & validkeywords);
   std::string GetNextWord();

 private:
   std::map<std::string, std::string> reservedkw;
};

} // lpmd

#endif


