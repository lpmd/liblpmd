//
//
//

#include <lpmd/cmdline.h>
#include <lpmd/util.h>

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

   std::list<Option> optlist;
   std::list<std::string> arglist;
};

UnknownCmdLineOption::UnknownCmdLineOption(const std::string name): Error("Unknown command line option: "+name) { }

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
 impl.optlist.push_back(opt);
}

void CommandArguments::Parse(int argc, char *argv[])
{
 CommandArgumentsImpl & impl = *clpimpl;
 Option curropt;
 std::list<std::string> optargs;
 for (int i=0;i<argc;++i)
 {
  if (!strcmp(argv[i], "-")) impl.arglist.push_back(std::string(argv[i]));
  else if ((strlen(argv[i]) > 1) && (argv[i][0] == '-') && (argv[i][1] == '-'))
  {
   // Opcion larga 
   std::string optname(&argv[i][2]);
   bool optfound = false;
   for (std::list<Option>::const_iterator it=impl.optlist.begin();it!=impl.optlist.end();++it)
   {
    if (optname == (*it).longname)
    {
     curropt = (*it);
     optfound = true;
     break;
    }
   }
   if (optfound == false) throw UnknownCmdLineOption("--"+optname);
   optargs = StringSplit< std::list<std::string> >(curropt.args);
   AssignParameter(curropt.longname, "true");
  }
  else if ((strlen(argv[i]) > 0) && (argv[i][0] == '-'))
  {
   // Opcion corta
   std::string optname(&argv[i][1]);
   bool optfound = false;
   for (std::list<Option>::const_iterator it=impl.optlist.begin();it!=impl.optlist.end();++it)
   {
    if (optname == (*it).shortname)
    {
     curropt = (*it);
     optfound = true;
     break;
    }
   }
   if (optfound == false) throw UnknownCmdLineOption("-"+optname);
   optargs = StringSplit< std::list<std::string> >(curropt.args);
   AssignParameter(curropt.longname, "true");
  }
  else
  {
   // Argumento de una opcion o argumento general del programa
   if (optargs.size() > 0)
   {
    std::string this_arg = optargs.front();
    optargs.pop_front();
    AssignParameter(curropt.longname+"-"+this_arg, std::string(argv[i]));
   }
   else impl.arglist.push_back(std::string(argv[i]));
  }
 }
}

std::list<std::string> CommandArguments::Arguments() const
{
 ;
 return clpimpl->arglist;
}

