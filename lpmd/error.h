//
//
//

#ifndef __LPMD_ERROR_H__
#define __LPMD_ERROR_H__

#include <exception>
#include <string>

namespace lpmd
{

 class Error: public std::exception
 {
  public:
    Error(const std::string description);
    ~Error() throw();

    const char * what() const throw();

    const std::string description;
 };

 //
 // Error hierarchy
 //
 class NotImplemented: public Error
 {
  public:
    NotImplemented(const std::string functionality);
 };
 
 class InvalidOperation: public Error
 {
  public:
    InvalidOperation(const std::string operation);
 };

 class RuntimeError: public Error
 {
  public:
    RuntimeError(const std::string description);
 };

 //
 //
 //
 class SystemError: public RuntimeError
 {
  public:
    SystemError(const std::string description);
 };

 class FileNotFound: public SystemError
 {
  public:
    FileNotFound(const std::string filename);
 };

 class SyntaxError: public RuntimeError
 {
  public:
    SyntaxError(const std::string description);
 };

 class PluginError: public RuntimeError
 {
  public:
    PluginError(const std::string pluginname, const std::string description);
 };

 class InvalidRequest: public RuntimeError
 {
  public:
    InvalidRequest(const std::string request);
 };

 class MissingComponent: public RuntimeError
 {
  public:
    MissingComponent(const std::string component);
 };

} // lpmd

#endif

