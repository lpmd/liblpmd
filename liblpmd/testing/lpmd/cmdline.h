//
//
//

#ifndef __LPMD_CMDLINE_H__
#define __LPMD_CMDLINE_H__

#include <lpmd/paramlist.h>
#include <lpmd/error.h>
#include <cstring>

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
  std::list<std::string> Arguments() const;
 
 private:
   class CmdLineParserImpl * clpimpl;
};

class UnknownCmdLineOption: public Error
{
 public:
   UnknownCmdLineOption(const std::string name);
};

} // lpmd

#endif


