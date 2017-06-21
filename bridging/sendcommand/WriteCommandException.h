#pragma once


namespace brewery {

  struct WriteCommandException : std::runtime_error {
  WriteCommandException(const std::string& error)
    : std::runtime_error("ERROR: Unable to write command: "+error) {
    }
  };
}
