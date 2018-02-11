/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Id sends back the identification string
   */

  struct Id {
    static void run();
  };


  /*
   * Process the ID command
   */

  inline void Id::run() {
    Uart::sendString(IdString,true);
  }
}
