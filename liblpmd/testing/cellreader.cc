//
//
//

#include <lpmd/cellreader.h>
#include <lpmd/simulationhistory.h>
#include <lpmd/storedconfiguration.h>

#include <fstream>

using namespace lpmd;

//
//

InvalidInput::InvalidInput(const std::string & filename): Error("Invalid input in file \""+filename+"\"") { }

//
//

void CellReader::Generate(Configuration & conf) const { Read(readfile, conf); }

void CellReader::Read(const std::string & filename, Configuration & conf) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadHeader(is);
 if (! ReadCell(is, conf)) throw InvalidInput(filename);
 #warning "Corregir sc.NumEspec()"
// sc.NumEspec();
}

void CellReader::ReadMany(const std::string & filename, SimulationHistory & hist) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadHeader(is);
 StoredConfiguration sconf;
 if (hist.Size() > 0) 
 {
  sconf = StoredConfiguration(hist[0]);
  hist.Clear();
 }
 while (1)
 {
  sconf.Atoms().Clear();
  if (ReadCell(is, sconf))
  {
   #warning "Corregir sc.NumEspec() y sc.AssignIndex()"
   //sc.NumEspec();
   //sc.AssignIndex();
   hist.Append(sconf);
  }
  else break;
 }
}

