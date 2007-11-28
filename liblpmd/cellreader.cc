//
//
//

#include <lpmd/cellreader.h>

#include <fstream>

using namespace lpmd;

void CellReader::Generate(SimulationCell & sc) const
{
 Read(readfile, sc);
}

void CellReader::Read(const std::string & filename, SimulationCell & sc) const
{
 std::ifstream is(filename.c_str());
 ReadHeader(is);
 ReadCell(is, sc);
}




