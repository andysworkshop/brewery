/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * VER sends back the version string
   */

  struct Version {
    static void run();
  };


  /*
   * Process the command
   */

  inline void Version::run() {
    Uart::sendString(VersionString,true);
  }
}
