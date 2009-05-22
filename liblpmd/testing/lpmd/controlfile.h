//
//
//

#ifndef __LPMD_CONTROLFILE_H__
#define __LPMD_CONTROLFILE_H__

#include <lpmd/map.h>

#include <map>

namespace lpmd
{

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

   Array<std::string> Parameters() const;

   void DeclareStatement(const std::string & cmd, const std::string & args);
 
   virtual void Read(std::istream & istr, const ParamList & options, const std::string inpfile);
   void Read(std::string inpfile, const ParamList & options);
   virtual int OnStatement(const std::string & name, const std::string & keywords, bool regular);

 protected:
   Array<std::string> words; // FIXME: hacer que words sea private, implementar funciones de acceso
   std::string MatchCommand(Array<std::string> & w);
   std::string ParseCommandArguments(const std::string & cmd, const std::string & validkeywords);
   std::string GetNextWord();

 private:
   Map * innermap;
   std::map<std::string, std::string> reservedkw;
   std::string filename;
};

} // lpmd

#endif

