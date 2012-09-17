//
//
//

#include <lpmd/cellgenerator.h>
#include <cassert>

using namespace lpmd;

CellGenerator::CellGenerator() { }

CellGenerator::~CellGenerator() { }

void CellGenerator::Generate(Configuration & conf) const { assert(&conf != 0);}//icc 869

