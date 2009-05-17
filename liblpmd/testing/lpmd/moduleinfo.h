/*
 *
 *
 *
 */

#ifndef __LPMD_MODULEINFO_H__
#define __LPMD_MODULEINFO_H__

#include <string>

namespace lpmd
{

class ModuleInfo
{
 public:
   std::string name;
   std::string id;
   std::string args;

   ModuleInfo(std::string modulename, std::string moduleid, std::string moduleargs);
};

}  // lpmd

#endif

