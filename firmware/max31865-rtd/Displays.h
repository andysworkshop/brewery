/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * DISPLAYS <RED/BLUE/BOTH/NONE> command
   */

  class Displays {

    public:
      static void run();
  };


  /*
   * execute the command
   */

  inline void Displays::run() {

    // get the parameter

    char *parameter=strtok(nullptr," ");

    if(parameter==nullptr)
      Uart::sendString(MissingParameterString,true);
    else {

      if(!strcasecmp("RED",parameter))
        Eeprom::Writer::displays(Max7221::Display::RED);
      else if(!strcasecmp("BLUE",parameter))
        Eeprom::Writer::displays(Max7221::Display::BLUE);
      else if(!strcasecmp("BOTH",parameter))
        Eeprom::Writer::displays(Max7221::Display::RED | Max7221::Display::BLUE);
      else if(!strcasecmp("NONE",parameter))
        Eeprom::Writer::displays(0);
      else {
        Uart::sendString(UnknownParameterString,true);
        return;
      }

      Uart::ok();
    }
  }
}
