//
//
//

#include <lpmd/cellwriter.h>
#include <lpmd/simulationhistory.h>

#include <fstream>

using namespace lpmd;

CellWriter::~CellWriter() { }

void CellWriter::Write(const std::string & filename, Configuration & conf) const
{
 std::ofstream os(filename.c_str());
 WriteHeader(os);
 WriteCell(os, conf);
}

void CellWriter::WriteMany(const std::string & filename, SimulationHistory & hist) const
{
 std::ofstream os(filename.c_str());
 WriteHeader(os);
 for (long i=0;i<hist.Size();++i) WriteCell(os, hist[i]);
}

std::string CellWriter::GetFile() { return writefile; }

