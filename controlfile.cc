//
//
//

#include <lpmd/controlfile.h>
#include <lpmd/util.h>
#include <lpmd/error.h>

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

using namespace lpmd;

class lpmd::ControlFileImpl
{
 public:
   std::map<std::string, std::string> reservedkw;
   std::string filename;
   Array<std::string> words;
   Array<std::string> blockheaders;
   Array<std::string> blockfooters;
   long int line_count;
   const ParamList * read_options;
   std::istream * istr;

   std::string GetNextWord();
   std::string MatchCommand();
};

//
//
//
ControlFile::ControlFile() { impl = new ControlFileImpl(); }

ControlFile::~ControlFile() { delete impl; }

void ControlFile::DeclareStatement(const std::string & cmd, const std::string & args) 
{ 
 impl->reservedkw[cmd] = args; 
}

void ControlFile::DeclareBlock(const std::string & name, const std::string & terminator)
{
 impl->blockheaders.Append(name);
 impl->blockfooters.Append(terminator);
}

int ControlFile::OnRegularStatement(const std::string & name, const std::string & keywords) 
{ 
 assert(name == name);
 assert(keywords == keywords);
 return 0; 
}

int ControlFile::OnNonRegularStatement(const std::string & name, const std::string & full_statement) 
{ 
 assert (&name != 0); //icc 869
 assert (&full_statement != 0); //icc 869
 return 1; 
}

int ControlFile::OnBlock(const std::string & name, const std::string & full_statement)
{
 assert (&name != 0); //icc 869
 assert (&full_statement != 0); //icc 869
 return 1;
}

void ControlFile::Read(const std::string & inpfile, const ParamList & options)
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

inline std::string AddBlockContent(const std::string & blockname, const std::string & terminator, std::istream & istr, Array<std::string> & tmpwords)
{
 std::string inpbuffer = (blockname+"block ");
 for (int z=1;z<tmpwords.Size();++z) inpbuffer += (tmpwords[z]+" ");
 while (1)
 {
  std::string line;
  if (istr.eof()) throw SyntaxError("\""+blockname+"\" block was not properly closed with \""+terminator+"\"");
  getline(istr, line);
  RemoveUnnecessarySpaces(line);
  if (line == terminator) break;
  inpbuffer += (line+" "); 
 }
 RemoveUnnecessarySpaces(inpbuffer);
 inpbuffer += "\n";
 return inpbuffer;
}

void ControlFile::ReadLine(const std::string & line)
{
  std::string tmp = line;
  const ParamList & options = *(impl->read_options);
  while (tmp[tmp.size()-1] == '\\')
  {
   std::string tmp2;
   getline(*(impl->istr), tmp2);
   tmp = tmp.substr(0, tmp.size()-1) + tmp2;
  }
  (impl->line_count)++;
  std::ostringstream strlnum;
  strlnum << impl->line_count;
  // Sustituye las opciones
  Array<Parameter> opts = options.Parameters();
  for (long int j=0;j<opts.Size();++j)
  {
   const std::string & jt = opts[j];
   std::string searchstr = "$("+(jt)+")";
   std::string::size_type idx = 0;
   while (true)
   {
    idx = tmp.find(searchstr, idx);
    if (idx == std::string::npos) break;
    tmp.replace(idx, searchstr.size(), options[jt]);
    idx += options[jt].length();
   }  
  }
  if (tmp.find("$(", 0) != std::string::npos)
    throw SyntaxError("Variable(s) undefined in input file \""+impl->filename+"\", line "+ToString(impl->line_count)+"\n  "+tmp);
  //
  impl->words = StringSplit(tmp);
  if (impl->words.Size() == 0) return;      // was continue
  if (impl->words[0][0] == '#') return;     // was continue
  std::string first_word = impl->words[0];
  std::string statement_args = impl->MatchCommand();
  if (statement_args == "") impl->words.Delete(0);
  int blockpos = -1;
  for (int q=0;q<impl->blockheaders.Size();++q)
  {
   if (first_word == (impl->blockheaders[q]+"block")) { blockpos = q; break; }
  }
  if (blockpos != -1)
  {
   if (OnBlock(impl->blockheaders[blockpos], tmp) != 0)
      throw SyntaxError("Invalid content of \""+impl->blockheaders[blockpos]+"\" block, in input file \""+impl->filename+"\", line "+ToString(impl->line_count)+"\n  "+tmp);
  }
  else
  {
   if (statement_args != "")
   {
    // Palabra clave de tipo regular, o no valida
    std::string st_keywords = ParseCommandArguments(*this, first_word, statement_args);
    if (OnRegularStatement(first_word, st_keywords) != 0)
       throw SyntaxError("Unexpected error in input file \""+impl->filename+"\", line "+ToString(impl->line_count)+"\n  "+tmp);
   }
   else 
   {
    if (OnNonRegularStatement(first_word, tmp) != 0)
       throw SyntaxError("Unexpected instruction was found in input file \""+impl->filename+"\", line "+ToString(impl->line_count)+"\n  "+tmp);
   }
  }
}

inline void RemoveComments(std::string & line)
{
 std::size_t pos = line.find_first_of('#');
 if (pos != std::string::npos) line = line.substr(0, pos);
}

void ControlFile::Read(std::istream & real_istr, const ParamList & options, const std::string & inpfile)
{
 //
 //
 //
 std::string inpbuffer, line;
 while (!real_istr.eof())
 {
  getline(real_istr, line);
  Array<std::string> tmpwords = StringSplit(line);
  if (tmpwords.Size() == 0) continue;
  bool foundblock = false;
  for (int q=0;q<impl->blockheaders.Size();++q)
  {
   if (tmpwords[0] == impl->blockheaders[q])
   {
    inpbuffer += AddBlockContent(impl->blockheaders[q], impl->blockfooters[q], real_istr, tmpwords);
    foundblock = true;
    break;
   }
  }
  if (!foundblock) inpbuffer += (line+"\n");
 } 
 //
 //
 //
 impl->istr = new std::istringstream(inpbuffer);
 std::string tmp;
 impl->line_count = 0;
 impl->filename = inpfile;
 impl->read_options = &options;
 while(getline(*(impl->istr), tmp)) 
 {
  RemoveComments(tmp);
  ReadLine(tmp);
 }
 delete impl->istr;
}

std::string ControlFile::ParseCommandArguments(Map & param, const std::string & cmd, const std::string & validkeywords)
{
 long int argcount = 0;
 std::string kvpairs;
 const Array<std::string> kvect = StringSplit(validkeywords);
 ParamList resolved;
 for (int q=0;q<kvect.Size();++q) resolved[kvect[q]] = "false";
 while (impl->words.Size() > 0)
 {
  std::string arg = impl->words[0]; 
  impl->words.Delete(0);
  Array<std::string> alist = StringSplit(arg, '=');
  if (alist.Size() == 1)
  {
   // El argumento actual no es de tipo keyword, se evaluara posicionalmente
   if (argcount >= kvect.Size()) continue;
   std::string keyword = kvect[argcount];
   std::string value = alist[0];
   param[cmd+"-"+keyword] = value;
   kvpairs += (keyword+" "+value+" ");
   resolved[keyword] = "true";
   argcount++;
  }
  else
  {
   // El argumento actual es de tipo keyword
   std::string keyword = alist[0];
   RemoveUnnecessarySpaces(keyword);
   std::string value = alist[alist.Size()-1];
   param[cmd+"-"+keyword] = value; 
   kvpairs += (keyword+" "+value+" ");
   resolved[keyword] = "true";
  }
  bool ready = true;
  for (int q=0;q<kvect.Size();++q) 
      if (bool(resolved[kvect[q]]) == false) ready = false;
  if (ready) break;
 }
 RemoveUnnecessarySpaces(kvpairs);
 return kvpairs;
}
//
// Methods in the private implementation class
//

std::string ControlFileImpl::GetNextWord()
{
 if (words.Size() == 0) throw SyntaxError("reading control file "+filename);
 std::string nextword = words[0];
 words.Delete(0);
 return nextword;
}

std::string ControlFileImpl::MatchCommand()
{
 std::string tmp;
 for (std::map<std::string, std::string>::const_iterator it=reservedkw.begin();it!=reservedkw.end();++it)
 {
  std::string key = it->first;
  Array<std::string> lt = StringSplit(key);
  bool badmatch = false;
  for (long int j=0;j<lt.Size();++j)
  {
   const std::string & ak = lt[j];
   const std::string & bk = words[j];
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
    words.Delete(0);
   }
   return reservedkw[key];
  }
 }
 return "";
}

