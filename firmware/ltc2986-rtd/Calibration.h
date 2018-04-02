/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Various calibration-related commands - store/return offsets, dates
   */

  class Calibration {

    protected:
      static void cal(Eeprom::Location which);
      static void calDate(Eeprom::Location which);

    public:
      static void red();
      static void blue();

      static void redDate();
      static void blueDate();
  };


  /*
   * set or return the red calibration offset
   */

  inline void Calibration::red() {
    cal(Eeprom::Location::REDCAL);
  }


  /*
   * set or return the blue calibration offset
   */

  inline void Calibration::blue() {
    cal(Eeprom::Location::BLUECAL);
  }


  /*
   * set or return either of the calibration offsets
   */

  inline void Calibration::cal(Eeprom::Location which) {

    // get the optional parameter

    double cal;
    char *parameter=strtok(nullptr," ");

    if(parameter==nullptr) {

      // respond with the current value

      if(which==Eeprom::Location::REDCAL)
        cal=Eeprom::Reader::redCal();
      else
        cal=Eeprom::Reader::blueCal();

      char buffer[100];
      sprintf(buffer,"\"%f\"",cal);

      Uart::sendString(buffer,false);
    }
    else {

      // store the new value

      cal=strtod(parameter,nullptr);

      if(which==Eeprom::Location::REDCAL)
        Eeprom::Writer::redCal(cal);
      else
        Eeprom::Writer::blueCal(cal);

      Uart::ok();
    }
  }


  /*
   * set or return the red calibration date
   */

  inline void Calibration::redDate() {
    calDate(Eeprom::Location::REDCALDATE);
  }


  /*
   * set or return the blue calibration date
   */

  inline void Calibration::blueDate() {
    calDate(Eeprom::Location::BLUECALDATE);
  }


  /*
   * set or return either of the calibration dates
   */

  inline void Calibration::calDate(Eeprom::Location which) {

    // get the optional parameter - a 32-bit Unix time_t

    uint32_t calDate;
    char *parameter=strtok(nullptr," ");

    if(parameter==nullptr) {

      // respond with the current value

      if(which==Eeprom::Location::REDCALDATE)
        calDate=Eeprom::Reader::redCalDate();
      else
        calDate=Eeprom::Reader::blueCalDate();

      char buffer[100];
      sprintf(buffer,"\"%lu\"",calDate);

      Uart::sendString(buffer,false);
    }
    else {

      // store the new value

      calDate=strtoul(parameter,nullptr,0);

      if(which==Eeprom::Location::REDCALDATE)
        Eeprom::Writer::redCalDate(calDate);
      else
        Eeprom::Writer::blueCalDate(calDate);

      Uart::ok();
    }
  }
}
