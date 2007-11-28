//
//
//

#ifndef __LPMD_CMDLINE_H__
#define __LPMD_CMDLINE_H__

#include <exception>
#include "paramlist.h"

namespace lpmd
{

class CmdLineParser: public ParamList
{
 public:
  //
  CmdLineParser();
  CmdLineParser(int argc, char *argv[]);
  ~CmdLineParser();

  //
  void DefineOption(const std::string & longname, const std::string & shortname, const std::string & args);

  //
  void Parse(int argc, char *argv[]);
 
 private:
   class CmdLineParserImpl * clpimpl;
};

class UnknownCmdLineOption: public std::exception
{
 public:
   UnknownCmdLineOption(const std::string & name);
   ~UnknownCmdLineOption() throw();

   const char * what() const throw();
 private:
   std::string optname;
};

} // lpmd

#endif


