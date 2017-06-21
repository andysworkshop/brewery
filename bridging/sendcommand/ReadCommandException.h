#pragma once


namespace brewery {

  struct ReadCommandException : std::runtime_error {
  ReadCommandException(const std::string& error)
    : std::runtime_error("ERROR: Unable to read command: "+error) {
    }
  };
}
