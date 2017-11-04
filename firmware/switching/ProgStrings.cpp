/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#include "Application.h"
#include "FirmwareVersion.h"


namespace brewery {

  /*
   * Constant strings
   */

  const char IdString[] PROGMEM =  "\"Andy's Workshop Brewery switching controller\"";
  const char VersionString[] PROGMEM =  "{\"hardware\":1,\"firmware\":" FIRMWARE_VERSION "}";
  const char CopyrightString[] PROGMEM = "\"Copyright (c) 2017 Andy Brown\"";
  const char CapabilitiesString[] PROGMEM = "{\"type\":\"switching\",\"relays\":[\"HEAT\",\"CHILL\",\"AUX1\"],\"triacs\":[\"AUX2\"]}";
  const char OKString[] PROGMEM =  "OK" ;
  const char UnknownCommandString[] PROGMEM = "ERROR:00:Unknown command";
  const char MissingParameterString[] PROGMEM = "ERROR:01:Missing parameter" ;
  const char UnknownParameterString[] PROGMEM = "ERROR:02:Unknown parameter";
  const char OutOfRangeString[] PROGMEM = "ERROR:03:Parameter out of range";
  const char InvalidStateString[] PROGMEM = "ERROR:04:Invalid controls state";
}
