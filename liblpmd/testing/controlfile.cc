//
//
//

#include <lpmd/controlfile.h>
#include <lpmd/util.h>
#include <lpmd/paramlist.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace lpmd;

//
//
//
InputError::InputError(const std::string desc, int nl, const std::string line): Error(desc+", line "+ToString<int>(nl)+"\n  "+line) { }

InputSyntaxError::InputSyntaxError(): Error("Syntax error on input file") { }

ControlFile::ControlFile(Map & m) { innermap = &m; }

ControlFile::~ControlFile() { }

void ControlFile::SetMap(Map & m) { innermap = &m; }

Map & ControlFile::GetMap() const { return (*innermap); }

bool ControlFile::Defined(const std::string & key) const { return innermap->Defined(key); }

void ControlFile::AssignParameter(const std::string & key, std::string value) { innermap->AssignParameter(key, value); }

std::string & ControlFile::operator[](const std::string & key) { return (*innermap)[key]; }

const std::string & ControlFile::operator[](const std::string & key) const { return (*innermap)[key]; } 

void ControlFile::Remove(const std::string & key) { innermap->Remove(key); }

Array<std::string> ControlFile::Parameters() const { return innermap->Parameters(); }

void ControlFile::DeclareStatement(const std::string & cmd, const std::string & args) { reservedkw[cmd] = args; }

//
//
//
std::string ControlFile::MatchCommand(Array<std::string> & w)
{
 std::string tmp;
 for (std::map<std::string, std::string>::const_iterator it=reservedkw.begin();it != reservedkw.end();++it)
 {
  std::string key = it->first;
  Array<std::string> lt = StringSplit(key);
  bool badmatch = false;
  for (long int j=0;j<lt.Size();++j)
  {
   const std::string & ak = lt[j];
   const std::string & bk = w[j];
   if (ak != bk) 
   {
    badmatch = true;
    break;
   }
  }
  if (badmatch == false) 
  {
   while (lt.Size() > 0) 
   {
    lt.Delete(0);
    w.Delete(0);
   }
   return reservedkw[key];
  }
 }
 return "";
}

//
//
//
std::string ControlFile::ParseCommandArguments(const std::string & cmd, const std::string & validkeywords)
{
 long int argcount = 0;
 std::string kvpairs = "";
 const Array<std::string> kvect = StringSplit(validkeywords);
 Map & param = (*this);
 while (words.Size() > 0)
 {
  std::string arg = words[0]; 
  words.Delete(0);
  Array<std::string> alist = StringSplit(arg, '=');
  if (alist.Size() == 1)
  {
   // El argumento actual no es de tipo keyword, se evaluara posicionalmente
   if (argcount >= kvect.Size()) continue;
   std::string keyword = kvect[argcount];
   std::string value = alist[0];
   param[cmd+"-"+keyword] = value;
   kvpairs += (keyword + " " + value + " ");
   argcount++;
  }
  else
  {
   // El argumento actual es de tipo keyword
   std::string keyword = alist[0];
   std::string value = alist[alist.Size()-1];
   param[cmd+"-"+keyword] = value; 
   kvpairs += (keyword + " " + value + " ");
  }
 }
 return kvpairs;
}

//
//
//
std::string ControlFile::GetNextWord()
{
 if (words.Size() == 0) throw InputSyntaxError();
 std::string nextword = words[0];
 words.Delete(0);
 return nextword;
}

//
//
//
int ControlFile::OnStatement(const std::string & name, const std::string & keywords, bool regular) { return 0; }

//
//
//
void ControlFile::Read(std::string inpfile, const ParamList & options)
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
void ControlFile::Read(std::istream & istr, const ParamList & options, const std::string inpfile)
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
  Array<std::string> opts = options.Parameters();
  for (long int j=0;j<opts.Size();++j)
  {
   const std::string & jt = opts[j];
   std::string searchstr = "$("+(jt)+")";
   std::string::size_type idx = 0;
   while (true)
   {
    idx = tmp.find(searchstr, idx);
    if (idx == std::string::npos) break;
    tmp.replace(idx, searchstr.size(), options.GetString(jt));
    idx += options.GetString(jt).length();
   }  
  }
  if (tmp.find("$(", 0) != std::string::npos) throw InputError("Variable(s) undefined in input file \""+inpfile+"\"", line_count, tmp);
  //
  words = StringSplit(tmp);
  if (words.Size() == 0) continue;
  if (words[0][0] == '#') continue;
  std::string first_word = words[0];
  std::string statement_args = MatchCommand(words);
  if (statement_args == "") words.Delete(0);
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


