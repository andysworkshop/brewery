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

    // get the parameter. there must be one

    char *parameter=strtok(nullptr," ");
    
    if(parameter==nullptr) {
      Uart::sendString(MissingParameterString,true);
      return;
    }

    // get the period and store in eeprom

    uint32_t period = atol(parameter);

    Eeprom::Writer::chillBlackout(period);
    Uart::ok();
  }
}
