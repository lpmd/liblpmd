//
//
//

#ifndef __LPMD_INPUTREADER_H__
#define __LPMD_INPUTREADER_H__

#include <lpmd/paramlist.h>
#include <lpmd/error.h>

#include <list>
#include <map>

namespace lpmd
{

class InputError: public Error
{
 public:
   InputError(const std::string desc, int nl, const std::string line);
};

class InputSyntaxError: public Error
{
 public:
   InputSyntaxError();
};

class InputFile: public ParamList
{
 public:
   InputFile();
   virtual ~InputFile();

   void DeclareStatement(const std::string & cmd, const std::string & args);
 
   virtual void Read(std::istream & istr, const ParamList & options, const std::string inpfile);
   void Read(std::string inpfile, const ParamList & options);
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


