/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Main program class
   */

  class Program {
   
  protected:
    Heater _heater;
    Chiller _chiller;
    Aux1 _aux1;
    uint8_t _currentMask{0};

  public:
    void run();
    void runCommand();
  };


  /*
   * Run this program and don't return
   */

  inline void Program::run() {

    // verify EEPROM

    Eeprom::verifyState();

    // turn everything off

    GpioHeater::reset();
    GpioChiller::reset();
    GpioAux1::reset();
    GpioAux2::reset();

    // enable watchdog on a 2 second timer

    wdt_enable(WDTO_2S);

    // enable global interrupt

    sei();

    for(;;) {

      // check for commands to process

      runCommand();

      // reset the watchdog

      wdt_reset();
    }
  }


  /*
   * Check UART and execute command
   */

  inline void Program::runCommand() {

    // check if there's a command ready to process

    if(!Uart::isCommandReady())
      return;

    // get the first token (the command)

    char *commandPtr=strtok(Uart::getCommandPtr()," ");

    if(!strcasecmp("HEAT",commandPtr))
      _heater.run(_currentMask);
    else if(!strcasecmp("CHILL",commandPtr))
      _chiller.run(_currentMask);
    else if(!strcasecmp("AUX1",commandPtr))
      _aux1.run(_currentMask);
    else if(!strcasecmp("AUX2",commandPtr))
      Aux2::run(_currentMask);
    else if(!strcasecmp("ID",commandPtr))
      Id::run();
    else if(!strcasecmp("COPY",commandPtr))
      Copyright::run();
    else if(!strcasecmp("VER",commandPtr))
      Version::run();
    else if(!strcasecmp("CAPS",commandPtr))
      Capabilities::run();
    else if(!strcasecmp("UPTIME",commandPtr))
      Uptime::run();
    else if(!strcasecmp("VALID",commandPtr))
      Valid::run();
    else
      Uart::sendString(UnknownCommandString,true);
  }
}
