#pragma once


namespace brewery {

  struct GetTerminalAttributesException : public std::runtime_error {

  GetTerminalAttributesException(const std::string& error)
    : std::runtime_error("ERROR: Unable to get terminal attributes: "+error) {
    }
  };
}
