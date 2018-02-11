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
   * Process the READINGS command
   */

  inline void Readings::run(RtdReadings& rtdReadings) {

    // shouldn't exceed 100 but be safe

    char buffer[150];

    sprintf(
      buffer,
      "{\"red\":{\"value\":\"%F\",\"code\":\"%u\"},\"blue\":{\"value\":\"%F\",\"code\":\"%u\"}}",
      rtdReadings.rtd2.temperature+Eeprom::Reader::redCal(),
      rtdReadings.rtd2.faultCode,
      rtdReadings.rtd1.temperature+Eeprom::Reader::blueCal(),
      rtdReadings.rtd1.faultCode);

    Uart::sendString(buffer,false);
  }
}
