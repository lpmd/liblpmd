//
//
//

#include <lpmd/cellreader.h>
#include <lpmd/simulationcell.h>

#include <fstream>

using namespace lpmd;

//
//

InvalidInput::InvalidInput(const std::string & filename): Error("Invalid input in file \""+filename+"\"") { }

//
//

void CellReader::Generate(SimulationCell & sc) const
{
 Read(readfile, sc);
}

void CellReader::Read(const std::string & filename, SimulationCell & sc) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadHeader(is);
 if (! ReadCell(is, sc)) throw InvalidInput(filename);
 sc.NumEspec();
}

void CellReader::ReadMany(const std::string & filename, std::vector<SimulationCell> & scs) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadHeader(is);
 SimulationCell sc;
 if (scs.size() > 0) 
 {
  sc = SimulationCell(scs[0]);
  scs.clear();
 }
 while (1)
 {
  sc.clear();
  bool st = ReadCell(is, sc);
  if (st)
  {
   sc.NumEspec();
   sc.AssignIndex();
   scs.push_back(sc);
  }
  else break;
 }
}

