/*
 * Andy's Workshop NXA66 controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  class Calibration {

    protected:
      static void cal(Eeprom::Location which);

    public:
      static void red();
      static void blue();
  };


  inline void Calibration::red() {
    cal(Eeprom::Location::REDCAL);
  }


  inline void Calibration::blue() {
    cal(Eeprom::Location::BLUECAL);
  }


  inline void Calibration::cal(Eeprom::Location which) {

    // get the parameter. there must be one

    char *parameter=strtok(nullptr," ");

    if(parameter==nullptr) {
      Uart::sendString(MissingParameterString,true);
      return;
    }

    double cal=strtod(parameter,nullptr);

    if(which==Eeprom::Location::REDCAL)
      Eeprom::Writer::redCal(cal);
    else
      Eeprom::Writer::blueCal(cal);

    Uart::ok();
  }
}
