//
//
//

#include <lpmd/inputfile.h>
#include <lpmd/util.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace lpmd;

//
//
//
InputError::InputError(const std::string desc, int nl, const std::string line): Error(desc+", line "+ToString<int>(nl)+"\n  "+line) { }

InputSyntaxError::InputSyntaxError(): Error("Syntax error on input file") { }

InputFile::InputFile() { }

InputFile::~InputFile() { }

void InputFile::DeclareStatement(const std::string & cmd, const std::string & args)
{
 reservedkw[cmd] = args;
}

//
//
//
std::string InputFile::MatchCommand(std::list<std::string> & w)
{
 std::string tmp;
 for (std::map<std::string, std::string>::const_iterator it=reservedkw.begin();it != reservedkw.end();++it)
 {
  std::string key = it->first;
  std::list<std::string> lt = ListOfTokens(key);
  bool badmatch = false;
  std::list<std::string>::const_iterator kt = w.begin();
  for (std::list<std::string>::const_iterator jt=lt.begin();jt != lt.end();++jt)
  {
   std::string ak = (*jt);
   std::string bk = (*kt);
   if (ak != bk) 
   {
    badmatch = true;
    break;
   }
   ++kt;
  }
  if (badmatch == false) 
  {
   while (lt.size() > 0) 
   {
    lt.pop_front();
    w.pop_front();
   }
   return reservedkw[key];
  }
 }
 return "";
}

//
//
//
std::string InputFile::ParseCommandArguments(const std::string & cmd, const std::string & validkeywords)
{
 unsigned int argcount = 0;
 std::string kvpairs = "";
 const std::vector<std::string> kvect = SplitTextLine(validkeywords);
 ParamList & param = (*this);
 while (words.size() > 0)
 {
  std::string arg = words.front(); 
  words.pop_front();
  std::list<std::string> alist = ListOfTokens(arg, '=');
  if (alist.size() == 1)
  {
   // El argumento actual no es de tipo keyword, se evaluara posicionalmente
   if (argcount >= kvect.size()) continue;
   std::string keyword = kvect[argcount];
   std::string value = alist.front();
   param[cmd+"-"+keyword] = value;
   kvpairs += (keyword + " " + value + " ");
   argcount++;
  }
  else
  {
   // El argumento actual es de tipo keyword
   std::string keyword = alist.front();
   std::string value = alist.back();
   param[cmd+"-"+keyword] = value; 
   kvpairs += (keyword + " " + value + " ");
  }
 }
 return kvpairs;
}

//
//
//
std::string InputFile::GetNextWord()
{
 if (words.size() == 0) throw InputSyntaxError();
 std::string nextword = words.front();
 words.pop_front();
 return nextword;
}

//
//
//
int InputFile::OnStatement(const std::string & name, const std::string & keywords, bool regular) { return 0; }

//
//
//
void InputFile::Read(std::string inpfile, const ParamList & options)
{
 if (inpfile == "-") Read(std::cin, options, "-");
 else
 {
  std::ifstream LoadSystem;
  LoadSystem.open(inpfile.c_str(), std::ios::in);
  if (!LoadSystem.good()) throw FileNotFound(inpfile);
  Read(LoadSystem, options, inpfile);
 }
}

//
//
//
void InputFile::Read(std::istream & istr, const ParamList & options, const std::string inpfile)
{
 std::string tmp;
 int line_count = 0;
 while(getline(istr, tmp))
 {
  while (tmp[tmp.size()-1] == '\\')
  {
   std::string tmp2;
   getline(istr, tmp2);
   tmp = tmp.substr(0, tmp.size()-1) + tmp2;
  }
  line_count++;
  std::ostringstream strlnum;
  strlnum << line_count;
  // Sustituye las opciones
  std::list<std::string> opts = options.Parameters();
  for (std::list<std::string>::const_iterator jt=opts.begin();jt!=opts.end();++jt)
  {
   std::string searchstr = "$("+(*jt)+")";
   std::string::size_type idx = 0;
   while (true)
   {
    idx = tmp.find(searchstr, idx);
    if (idx == std::string::npos) break;
    tmp.replace(idx, searchstr.size(), options.GetString(*jt));
    idx += options.GetString(*jt).length();
   }  
  }
  if (tmp.find("$(", 0) != std::string::npos) throw InputError("Variable(s) undefined in input file \""+inpfile+"\"", line_count, tmp);
  //
  words = ListOfTokens(tmp);
  if (words.size() == 0) continue;
  if (words.front()[0] == '#') continue;
  std::string first_word = words.front();
  std::string statement_args = MatchCommand(words);
  if (statement_args == "") words.pop_front();
  if (statement_args != "")
  {
   // Palabra clave de tipo regular, o no valida
   std::string kvpairs = ParseCommandArguments(first_word, statement_args);
   int st = OnStatement(first_word, kvpairs, true);
   if (st != 0) throw InputError("Unexpected error in input file \""+inpfile+"\"", line_count, tmp);
  }
  else 
  {
   int st = OnStatement(first_word, "", false);
   if (st == 1) throw InputError("Unexpected instruction was found in input file \""+inpfile+"\"", line_count, tmp);
  }
 }

}


