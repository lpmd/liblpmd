//
//
//

#include <lpmd/cellwriter.h>
#include <lpmd/simulationcell.h>

#include <fstream>

using namespace lpmd;

CellWriter::~CellWriter() { }

void CellWriter::Write(const std::string & filename, SimulationCell & sc) const
{
 std::ofstream os(filename.c_str());
 WriteHeader(os);
 WriteCell(os, sc);
}

void CellWriter::WriteMany(const std::string & filename, std::vector<SimulationCell> & scs) const
{
 std::ofstream os(filename.c_str());
 WriteHeader(os);
 for (unsigned long i=0;i<scs.size();++i)
 {
  WriteCell(os, scs[i]);
 }
}

std::string CellWriter::GetFile() { return writefile; }

