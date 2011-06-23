//
//
//

#include <lpmd/cellreader.h>
#include <lpmd/simulationhistory.h>
#include <lpmd/storedconfiguration.h>
#include <lpmd/session.h>
#include <lpmd/error.h>

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

bool CellReader::SkipCell(std::istream & is) const
{
 // Not implemented! 
 throw NotImplemented("SkipCell");
}

void CellReader::ReadMany(const std::string & filename, SimulationHistory & hist, const Stepper & stepper, bool skipheader) const
{
 std::ifstream is(filename.c_str());
 if (! is.good()) throw FileNotFound(filename);
 ReadMany(is, hist, stepper, skipheader);
}

void CellReader::ReadMany(std::istream & inputstream, SimulationHistory & hist, const Stepper & stepper, bool skipheader) const
{
 if (!skipheader) ReadHeader(inputstream);
 StoredConfiguration sconf;
 if (hist.Size() > 0) 
 {
  sconf = StoredConfiguration(hist[0]);
  hist.Clear();
 }
 long nconf = 0;
 while (1)
 {
  sconf.Atoms().Clear();
  if (stepper.IsActiveInStep(nconf))
  {
   if (!ReadCell(inputstream, sconf)) break;
   GlobalSession.DebugStream() << "-> Read configuration " << nconf << "\n";
   hist.Append(sconf);
   if (sconf.Have(sconf, Tag("level"))) 
      hist[hist.Size()-1].SetTag(hist[hist.Size()-1], Tag("level"), sconf.GetTag(sconf, Tag("level")));
  }
  else 
  {
   if (!SkipCell(inputstream)) break;
   GlobalSession.DebugStream() << "-> Skipped configuration " << nconf << "\n";
  }
  nconf++;
 }
}

