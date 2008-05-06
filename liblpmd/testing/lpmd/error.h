//
//
//

#ifndef __LPMD_ERROR_H__
#define __LPMD_ERROR_H__

#include <exception>
#include <string>

namespace lpmd
{

 //
 //
 //
 class Error: public std::exception
 {
  public:
    Error(const std::string msg);
    ~Error() throw();

    const char * what() const throw();

  private:  
    std::string * message;
 };

 //
 //
 //
 class FileNotFound: public Error
 {
  public:
    FileNotFound(const std::string filename);
 };

} // lpmd

#endif

