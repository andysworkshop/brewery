/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
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
