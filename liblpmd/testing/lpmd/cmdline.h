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

class CommandArguments: public ParamList
{
 public:
  //
  CommandArguments();
  CommandArguments(int argc, char *argv[]);
  ~CommandArguments();

  //
  void DefineOption(const std::string & longname, const std::string & shortname, const std::string & args);

  //
  void Parse(int argc, char *argv[]);
  std::list<std::string> Arguments() const;
 
 private:
   class CommandArgumentsImpl * clpimpl;
};

class UnknownCmdLineOption: public Error
{
 public:
   UnknownCmdLineOption(const std::string name);
};

} // lpmd

#endif


