//
//
//

#ifndef __LPMD_CONTROLFILE_H__
#define __LPMD_CONTROLFILE_H__

#include <lpmd/paramlist.h>

namespace lpmd
{

class ControlFile: public ParamList
{
 public:
   ControlFile();
   virtual ~ControlFile();

   void DeclareStatement(const std::string & name, const std::string & args);
   void DeclareBlock(const std::string & name, const std::string & terminator);
 
   void Read(const std::string & filename, const ParamList & options);

   virtual void Read(std::istream & istr, const ParamList & options, const std::string & filename="Unnamed");

 protected:
   std::string ParseCommandArguments(Map & param, const std::string & cmd, const std::string & validkeywords);
   virtual int OnRegularStatement(const std::string & name, const std::string & keywords);
   virtual int OnNonRegularStatement(const std::string & name, const std::string & full_statement);
   virtual int OnBlock(const std::string & name, const std::string & full_statement);

 private:
   class ControlFileImpl * impl;
};

} // lpmd

#endif

