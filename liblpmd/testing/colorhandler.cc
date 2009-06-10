/*
 *
 *
 *
 */

#include <lpmd/colorhandler.h>
#include <lpmd/basicatom.h>
#include <lpmd/error.h>

#include <map>

using namespace lpmd;

class lpmd::ColorHandlerImpl
{
 public:
   ColorHandlerImpl() { }

   std::map<const BasicAtom *, Color> colormap;
};

lpmd::ColorHandlerImpl ColorHandler::impl = ColorHandlerImpl();

ColorHandler::ColorHandler() { }

bool ColorHandler::HaveColor(const BasicAtom & at) { return (impl.colormap.count(&at) > 0); }

Color & ColorHandler::ColorOfAtom(const BasicAtom & at)
{
 try { return impl.colormap[&at]; }
 catch(Error & e) { throw RuntimeError("Atom has no color"); }
}

