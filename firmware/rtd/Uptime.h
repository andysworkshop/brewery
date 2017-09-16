/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Uptime sends back the millisecond timer
   */

  struct Uptime {
    static void run();
  };


  /*
   * Process the UPTIME command
   */

  inline void Uptime::run() {

    static char buffer[15];

    sprintf(buffer,"OK:%lu\r\n",MillisecondTimer::millis());
    Uart::sendString(buffer,false);
  }
}
