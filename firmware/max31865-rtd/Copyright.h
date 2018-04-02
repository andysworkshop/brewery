/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * COPY sends back the copyright string
   */

  struct Copyright {
    static void run();
  };


  /*
   * Process the command
   */

  inline void Copyright::run() {
    Uart::sendString(CopyrightString,true);
  }
}
