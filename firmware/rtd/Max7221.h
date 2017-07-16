/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  extern const uint8_t charTable [];

  /*
   * Handle interaction with the Max7221 SPI IC.
   */

  class Max7221 {

    public:
      
      /*
       * Register addresses
       */
      
      enum Register {
        NOP = 0,
        DIGIT0 = 1,
        DIGIT1 = 2,
        DIGIT2 = 3,
//        DIGIT3 = 4,     // not present on this display
        DIGIT4 = 5,
        DIGIT5 = 6,
        DIGIT6 = 7,
//        DIGIT7 = 8,     // not present on this display
        DECODE_MODE = 9,
        INTENSITY = 10,
        SCAN_LIMIT = 11,
        SHUTDOWN = 12,
        TEST_MODE = 15
      };

      enum Display {
        RED  = 0x1,        // 3 digits
        BLUE = 0x2         // 3 digits
      };

    public:
      static void setup();

      static void writeByte(Register reg,uint8_t byte);
      static void writeByte(int reg,uint8_t byte);
      static void segmentMode();
      static void shutdown();
      static void wakeup();
      static void intensity(uint8_t level);

      static void clearDisplay(Display display);
      static void displayText(Display display,const char *str);
  };


  /*
   * Inititialise the device
   */

  inline void Max7221::setup() {

    // self test for 1 second (all digits on)

    writeByte(TEST_MODE, 1);
    MillisecondTimer::delay(1000);
    writeByte(TEST_MODE, 0);

    // enable all digits

    writeByte(SCAN_LIMIT,6);

    // set segment mode

    segmentMode();

    // blank all digits

    clearDisplay(Display::RED);
    clearDisplay(Display::BLUE);

    // mid intensity

    intensity(Eeprom::Reader::intensity());

    // turn on

    wakeup();
  }


  inline void Max7221::segmentMode() {
    writeByte(DECODE_MODE,0);
  }
  
  inline void Max7221::shutdown() {
    writeByte(SHUTDOWN,0);
  }

  inline void Max7221::wakeup() {
    writeByte(SHUTDOWN,1);
  }

  inline void Max7221::intensity(uint8_t level) {
    writeByte(INTENSITY,level);
  }

  inline void Max7221::writeByte(int reg,uint8_t byte) {
    writeByte(static_cast<Register>(reg),byte);
  }

  inline void Max7221::writeByte(Register reg, uint8_t byte) {
    GpioSpiCsLed::reset();
    GpioSpiMosi::writeByte(reg);
    GpioSpiMosi::writeByte(byte);
    GpioSpiCsLed::set();
  }

  
  /*
   * clear down the display
   */

  inline void Max7221::clearDisplay(Display display) {

    // get first digit

    int digit=static_cast<int>(display==BLUE ? DIGIT4 : DIGIT0);
    
    for(int i=digit;i<digit+3;i++)
      writeByte(i,0);
  }


  /*
   * Print out text to the display. str must not be more than 3
   * characters
   */

  inline void Max7221::displayText(Display display,const char *str) {

    int digit=static_cast<int>(display==BLUE ? DIGIT4 : DIGIT0);
    uint8_t count;

    count=0;
    
    for(const char *ptr=str;*ptr;ptr++) {
       
      uint8_t value=pgm_read_byte_near(charTable+*ptr); 
      if(ptr[1]=='.') {
        value|=0x80;
        ptr++;
      }

      writeByte(digit++,value);
      count++;
    }

    while(count<3) {
      writeByte(digit++,0);
      count++;
    }
  }
}
