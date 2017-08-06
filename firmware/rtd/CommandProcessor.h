/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {


  /*
   * The command processor checks for commands arriving and dispatches them
   * to the appropriate handler implementation
   */

  struct CommandProcessor {
    static void run(RtdReadings& rtdReadings);
  };


  /*
   * Main loop, check for and dispatch command
   */

  inline void CommandProcessor::run(RtdReadings& rtdReadings) {

    // check if there's a command ready to process

    if(!Uart::isCommandReady())
      return;

    // get the first token

    char *commandPtr=strtok(Uart::getCommandPtr()," ");

    if(!strcasecmp("ID",commandPtr))
      Id::run();
    else if(!strcasecmp("CAPS",commandPtr))
      Capabilities::run();
    else if(!strcasecmp("VER",commandPtr))
      Version::run();
    else if(!strcasecmp("COPY",commandPtr))
      Copyright::run();
    else if(!strcasecmp("UPTIME",commandPtr))
      Uptime::run();
    else if(!strcasecmp("READINGS",commandPtr))
      Readings::run(rtdReadings);
    else if(!strcasecmp("RCAL",commandPtr))
      Calibration::red();
    else if(!strcasecmp("BCAL",commandPtr))
      Calibration::blue();
    else if(!strcasecmp("RCALDATE",commandPtr))
      Calibration::redDate();
    else if(!strcasecmp("BCALDATE",commandPtr))
      Calibration::blueDate();
    else if(!strcasecmp("SERIAL",commandPtr))
      Serial::run();
    else
      Uart::sendString(UnknownCommandString,true);
  }
}
