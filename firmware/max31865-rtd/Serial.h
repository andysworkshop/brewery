/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Serial sends back the serial number (generated and stored in serialnumber.txt then
   * written to EEPROM.
   */

  struct Serial {
    static void run();
  };


  /*
   * Process the command
   */

  inline void Serial::run() {
    char serial[100];
    sprintf(serial,"\"%lu\"",Eeprom::Reader::serial());
    Uart::sendString(serial,false);
  }
}
