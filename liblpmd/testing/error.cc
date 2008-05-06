//
//
//

#include <lpmd/error.h>

using namespace lpmd;

Error::Error(const std::string msg) { message = new std::string(msg); }

Error::~Error() throw() { delete message; }

const char * Error::what() const throw() { return message->c_str(); }

//
//

FileNotFound::FileNotFound(const std::string filename): Error("File \""+filename+"\" not found.") { }

