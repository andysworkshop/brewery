#pragma once


namespace brewery {

  struct SetTerminalAttributesException : std::runtime_error {
    SetTerminalAttributesException(const std::string& error)
      : std::runtime_error("ERROR: Unable to set terminal attributes: "+error) {
    }
  };
}
