/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#include "Application.h"


namespace brewery {

  /*
   * Constant strings
   */

  const char IdString[] PROGMEM =  "\"Andy's Workshop Brewery RTD temperature sensors\"";
  const char CapabilitiesString[] PROGMEM = "{\"type\":\"rtd\",\"names\": [ \"RED\",\"GREEN\"]}";
  const char VersionString[] PROGMEM =  "{\"hardware\":1,\"firmware\":1}";
  const char OKString[] PROGMEM =  "\"OK\"\r\n" ;
  const char UnknownCommandString[] PROGMEM = "ERROR:00:Unknown command";
  const char MissingParameterString[] PROGMEM = "ERROR:01:Missing parameter" ;
  const char UnknownParameterString[] PROGMEM = "ERROR:02:Unknown parameter";
  const char OutOfRangeString[] PROGMEM = "ERROR:03:Parameter out of range";
  const char CopyrightString[] PROGMEM = "\"Copyright (c) 2017 Andy Brown\"";
}
