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


      /*
       * Display one of the readings on a display after adjusting the raw
       * reading with the appropriate calibration value
       */

      void display(Max7221::Display display) {

        char buffer[20];
        double adjusted=temperature;

        // adjust the temperature with the calibration value

        if(display==Max7221::Display::BLUE)
          adjusted+=Eeprom::Reader::blueCal();
        else
          adjusted+=Eeprom::Reader::redCal();

        if(faultCode!=0)                    // display "Err" if fault code present
          Max7221::displayText(display,"Err");
        else if(adjusted<=-10)              // display "lo" if too far negative
          Max7221::displayText(display,"LO ");
        else if(adjusted>=1000)             // display "hi" if out of range
          Max7221::displayText(display,"HI ");
        else {

          if(adjusted>=100)     // display an integer if >100
            sprintf(buffer,"%d",(int)adjusted);
          else                          // display a fraction if <100
            sprintf(buffer,"%4.1f",adjusted);

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
