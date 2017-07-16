/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Encapsulate the RTD readings
   */

  struct RtdReadings {
    
    struct Result {
      uint8_t faultCode;
      double temperature;
      double directAdcReading;
      int32_t directAdcCode;

      /*
       * Display one of the readings on a display
       */

      void display(Max7221::Display display) {

        char buffer[20];

        if((faultCode & 0xfe)!=0)           // display "Err" if fault code present
          Max7221::displayText(display,"Err");
        else if(temperature<0)              // display "lo" if negative
          Max7221::displayText(display,"lo ");
        else if(temperature>=1000)          // display "out" if out of range
          Max7221::displayText(display,"out");
        else {

          if(temperature>=100)     // display an integer if >100
            sprintf(buffer,"%d",(int)temperature);
          else                          // display a fraction if <100
            sprintf(buffer,"%4.1f",temperature);

          Max7221::displayText(display,buffer);
        }
      }
    } rtd1,rtd2;
  
    
    /*
     * Display both readings on the display
     */

    void display() {

      uint8_t displays=Eeprom::Reader::displays();

      if((displays & Max7221::Display::BLUE)==Max7221::Display::BLUE)
        rtd1.display(Max7221::Display::BLUE);
      else
        Max7221::clearDisplay(Max7221::Display::BLUE);
  
      if((displays & Max7221::Display::RED)==Max7221::Display::RED)
        rtd2.display(Max7221::Display::RED);
      else
        Max7221::clearDisplay(Max7221::Display::RED);
    }
  };
}
