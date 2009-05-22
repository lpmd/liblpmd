//
//
//

#include <lpmd/cmdline.h>
#include <lpmd/util.h>
#include <lpmd/error.h>

#include <iostream>
#include <list>

using namespace lpmd;

//
class Option
{
 public:
   std::string longname;
   std::string shortname;
   std::string args;
};

//
class lpmd::CommandArgumentsImpl
{
 public:
   CommandArgumentsImpl() { };
   ~CommandArgumentsImpl() { };

   Array<Option> optlist;
   Array<std::string> arglist;
};

//
//
//
CommandArguments::CommandArguments()
{
 clpimpl = new CommandArgumentsImpl();
}

CommandArguments::CommandArguments(int argc, char *argv[])
{
 clpimpl = new CommandArgumentsImpl();
 Parse(argc, argv);
}

CommandArguments::~CommandArguments() 
{ 
 delete clpimpl;
}

void CommandArguments::DefineOption(const std::string & longname, const std::string & shortname, const std::string & args)
{
 CommandArgumentsImpl & impl = *clpimpl;
 Option opt;
 opt.longname = longname;
 opt.shortname = shortname;
 opt.args = args;
 impl.optlist.Append(opt);
}

void CommandArguments::Parse(int argc, char *argv[])
{
 CommandArgumentsImpl & impl = *clpimpl;
 Option curropt;
 Array<std::string> optargs;
 for (int i=0;i<argc;++i)
 {
  if (!strcmp(argv[i], "-")) impl.arglist.Append(std::string(argv[i]));
  else if ((strlen(argv[i]) > 1) && (argv[i][0] == '-') && (argv[i][1] == '-'))
  {
   // Opcion larga 
   std::string optname(&argv[i][2]);
   bool optfound = false;
   for (int i=0;i<impl.optlist.Size();++i)
   {
    if (optname == impl.optlist[i].longname)
    {
     curropt = impl.optlist[i];
     optfound = true;
     break;
    }
   }
   if (optfound == false) throw SyntaxError("Unknown command line option, --"+optname);
   optargs = StringSplit(curropt.args);
   AssignParameter(curropt.longname, "true");
  }
  else if ((strlen(argv[i]) > 0) && (argv[i][0] == '-'))
  {
   // Opcion corta
   std::string optname(&argv[i][1]);
   bool optfound = false;
   for (int i=0;i<impl.optlist.Size();++i)
   {
    if (optname == impl.optlist[i].shortname)
    {
     curropt = impl.optlist[i];
     optfound = true;
     break;
    }
   }
   if (optfound == false) throw SyntaxError("Unknown command line option, -"+optname);
   optargs = StringSplit(curropt.args);
   AssignParameter(curropt.longname, "true");
  }
  else
  {
   // Argumento de una opcion o argumento general del programa
   if (optargs.Size() > 0)
   {
    std::string this_arg = optargs[0];
    optargs.Delete(0);
    AssignParameter(curropt.longname+"-"+this_arg, std::string(argv[i]));
   }
   else impl.arglist.Append(std::string(argv[i]));
  }
 }
}

Array<std::string> CommandArguments::Arguments() const {  return clpimpl->arglist; }

