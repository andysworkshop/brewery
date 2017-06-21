#pragma once


namespace brewery {

  struct OpenPortException : std::runtime_error {
    
  OpenPortException(const std::string& port,const std::string& error)
    : std::runtime_error("ERROR: Unable to open port "+port+": "+error) {
    }
  };
}
