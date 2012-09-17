//
//
//

#include <lpmd/error.h>

using namespace lpmd;

//
//
//

Error::Error(const std::string desc): description(desc) { }
Error::~Error() throw () { }

const char * Error::what() const throw() { return description.c_str(); }

//
//
//

NotImplemented::NotImplemented(const std::string functionality): Error(functionality+" is not implemented.") { }

InvalidOperation::InvalidOperation(const std::string operation): Error(operation+" is not permitted.") { }
 
RuntimeError::RuntimeError(const std::string description): Error(description) { }

//
//
//
SystemError::SystemError(const std::string description): RuntimeError(description) { }

SyntaxError::SyntaxError(const std::string description): RuntimeError(description) { }

PluginError::PluginError(const std::string pluginname, const std::string description): RuntimeError("(in plugin "+pluginname+"): "+description) { }

InvalidRequest::InvalidRequest(const std::string request): RuntimeError(request+" not found.") { }

MissingComponent::MissingComponent(const std::string component): RuntimeError(component+" is needed for the process.") { }

//
//
//

FileNotFound::FileNotFound(const std::string filename): SystemError("File "+filename+" not found.") { }

