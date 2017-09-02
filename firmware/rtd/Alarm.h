/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * ALARM ON/OFF command (switches the alarm LED on/off)
   */

  class Alarm {

    public:
      static void run();
  };


  /*
   * execute the command
   */

  inline void Alarm::run() {

    // get the parameter

    char *parameter=strtok(nullptr," ");

    if(parameter==nullptr)
      Uart::sendString(MissingParameterString,true);
    else {

      if(!strcasecmp("ON",parameter))
        GpioAlarm::set();
      else if(!strcasecmp("OFF",parameter))
        GpioAlarm::reset();
      else {
        Uart::sendString(UnknownParameterString,true);
        return;
      }

      Uart::ok();
    }
  }
}
