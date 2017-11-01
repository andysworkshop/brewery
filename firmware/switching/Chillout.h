/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * CHILLOUT <milliseconds> sets the minimum period that the CHILL relay must stay switched
   * off before it can be switched back on again
   */

  struct Chillout {
    static void run();
  };


  /*
   * Process the command
   */

  inline void Chillout::run() {

    // get the parameter. if none, echo the current value

    char *parameter=strtok(nullptr," ");
    
    if(parameter==nullptr) {
      char buffer[100];
      sprintf(buffer,"\"%lu\"",Eeprom::Reader::chillBlackout());
      Uart::sendString(buffer,false);
      return;
    }

    // get the period and store in eeprom

    uint32_t period = atol(parameter);

    Eeprom::Writer::chillBlackout(period);
    Uart::ok();
  }
}
