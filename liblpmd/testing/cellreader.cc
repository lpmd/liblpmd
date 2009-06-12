//
//
//

#include <lpmd/cellreader.h>
#include <lpmd/simulationhistory.h>
#include <lpmd/storedconfiguration.h>

#include <fstream>
#include <memory>

using namespace lpmd;

//
//

void CellReader::Generate(Configuration & conf) const { Read(readfile, conf); }

void CellReader::Read(const std::string & filename, Configuration & conf) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadHeader(is);
 if (! ReadCell(is, conf)) throw SyntaxError("Invalid input in file \""+filename+"\"");
}

void CellReader::ReadMany(const std::string & filename, SimulationHistory & hist, bool skipheader) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadMany(is, hist, skipheader);
}

void CellReader::ReadMany(std::istream & inputstream, SimulationHistory & hist, bool skipheader) const
{
 if (!skipheader) ReadHeader(inputstream);
 StoredConfiguration sconf;
 if (hist.Size() > 0) 
 {
  sconf = StoredConfiguration(hist[0]);
  hist.Clear();
 }
 while (1)
 {
  sconf.Atoms().Clear();
  if (ReadCell(inputstream, sconf))
  {
   hist.Append(sconf);
   if (sconf.Have(sconf, Tag("level"))) 
      hist[hist.Size()-1].SetTag(hist[hist.Size()-1], Tag("level"), sconf.GetTag(sconf, Tag("level")));
  }
  else break;
 }
}

