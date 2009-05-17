//
//
//

#ifndef __LPMD_CONTROLFILE_H__
#define __LPMD_CONTROLFILE_H__

#include <lpmd/error.h>
#include <lpmd/map.h>

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

class ParamList; // forward
class ControlFile: public Map
{
 public:
   // 
   ControlFile(Map & m);
   virtual ~ControlFile();

   void SetMap(Map & m);

   Map & GetMap() const;

   // Implementacion de metodos de Map
   bool Defined(const std::string & key) const;

   void AssignParameter(const std::string & key, std::string value);

   std::string & operator[](const std::string & key);

   const std::string & operator[](const std::string & key) const;   
 
   void Remove(const std::string & key);

   std::list<std::string> Parameters() const;

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
   Map * innermap;
   std::map<std::string, std::string> reservedkw;
};

} // lpmd

#endif


