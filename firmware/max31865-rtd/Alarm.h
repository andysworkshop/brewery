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
      static void setLed();
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
        Eeprom::Writer::alarmState(AlarmState::ON);
      else if(!strcasecmp("OFF",parameter))
        Eeprom::Writer::alarmState(AlarmState::OFF);
      else if(!strcasecmp("FLASH",parameter))
        Eeprom::Writer::alarmState(AlarmState::FLASH);
      else {
        Uart::sendString(UnknownParameterString,true);
        return;
      }

      setLed();
      Uart::ok();
    }
  }


  /*
   * Set the LED from the EEPROM value
   */

  inline void Alarm::setLed() {

    switch(Eeprom::Reader::alarmState()) {

      case AlarmState::ON:
        AlarmFlasher::off();
        GpioAlarm::set();
        break;

      case AlarmState::FLASH:
        AlarmFlasher::on();
        break;

      default:
        AlarmFlasher::off();
        GpioAlarm::reset();
        break;
    }
  }
}
