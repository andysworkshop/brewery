/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Readings sends back the sensor readings
   */

  struct Readings {
    static void run(RtdReadings& rtdReadings);
  };


  /*
   * Process the ID command
   */

  inline void Readings::run(RtdReadings& rtdReadings) {

    static char buffer[100];

    sprintf(buffer,"GREEN:%f:%u;RED:%f:%u\r\n",
      rtdReadings.rtd1.temperature,
      rtdReadings.rtd1.faultCode,
      rtdReadings.rtd2.temperature,
      rtdReadings.rtd2.faultCode);
    
    Uart::sendString(buffer,false);
  }
}
