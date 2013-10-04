/*
 *
 *
 *
 */

#include <lpmd/moduleinfo.h>

using namespace lpmd;

ModuleInfo::ModuleInfo(std::string modulename, std::string moduleid, std::string moduleargs)
{
 name = modulename;
 id = moduleid;
 args = moduleargs;
}

