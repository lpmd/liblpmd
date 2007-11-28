//
//
//

#include <lpmd/cellwriter.h>

using namespace lpmd;

CellWriter::~CellWriter() { }

void CellWriter::Write(const std::string & filename, SimulationCell & sc) const
{
 std::ofstream os(filename.c_str());
 WriteHeader(os);
 WriteCell(os, sc);
}

std::string CellWriter::GetFile() { return writefile; }

