//
//
//

#ifndef __LPMD_CMDLINE_H__
#define __LPMD_CMDLINE_H__

#include <lpmd/paramlist.h>
#include <cstring>

namespace lpmd
{

class CommandArguments: public ParamList
{
 public:
  CommandArguments();
  ~CommandArguments();

  void DefineOption(const std::string & longname, const std::string & shortname, const std::string & args);

  void Parse(int argc, const char ** argv);
  Array<std::string> Arguments() const;
 
 private:
   class CommandArgumentsImpl * clpimpl;
};

}  // lpmd

#endif

