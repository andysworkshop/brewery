/*
 * Andy's Workshop Brewery RTD temperature controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * GPIO ports
   */

  struct GPIOB {
    enum {
      Port = 0x05,
      Dir  = 0x04,
      Pin  = 0x03
    };
  };

  struct GPIOC {
    enum {
      Port = 0x08,
      Dir  = 0x07,
      Pin  = 0x06
    };
  };

  struct GPIOD {
    enum {
      Port = 0x0b,
      Dir  = 0x0a,
      Pin  = 0x09
    };
  };


  /*
   * Base template for all GPIO pins. Provide support for set/reset
   */

  template<uint8_t TPort,uint8_t TPin>
  struct GpioPin {
   
    /*
     * Immediately set a pin to HIGH
     */

    static void set() {
      asm volatile(
        "  sbi  %[port],%[pin]  \n\t"
        :: [port] "I" (TPort),
           [pin]  "I" (TPin)
      );
    }


    /*
     * Immediately set a pin to LOW
     */

    static void reset() {
      asm volatile(
        "  cbi  %[port],%[pin]  \n\t"
        :: [port] "I" (TPort),
           [pin]  "I" (TPin)
      );
    }


    /*
     * Change state to the given state immediately
     */

    static void changeTo(bool state) {
      if(state)
        set();
      else
        reset();
    }
  };


  /*
   * GPIO output, provide support for init
   */

  template<class TPort,uint8_t TPin>
  struct GpioOutputPin : GpioPin<TPort::Port,TPin> {

    /*
     * Setup the port pin by setting the direction bit in the port register
     */

    static void setup() {    
      asm volatile(
        "  sbi  %[port],%[pin]  \n\t"
        :: [port] "I" (TPort::Dir),
           [pin]  "I" (TPin)
      );
    }
  };


  /*
   * GPIO input, provide support for init, read
   */

  template<class TPort,uint8_t TPin>
  struct GpioInputPin : GpioPin<TPort::Port,TPin> {

    /*
     * Setup the pin by clearing the direction bit in the port register
     */

    static void setup() {    
      asm volatile(
        "  cbi  %[port],%[pin]  \n\t"
        :: [port] "I" (TPort::Dir),
           [pin]  "I" (TPin)
      );
    }


    /*
     * Read the current value of an input pin
     */

    static bool read() {
      
      uint8_t r;

      asm volatile(
          "  clr  %[result]       \n\t"       // result = 0
          "  sbic %[port],%[pin]  \n\t"       // skip next if port bit is clear
          "  inc  %[result]       \n\t"       // result = 1
        : [result] "=r" (r)
        : [port]   "I"  (TPort::Pin),
          [pin]    "I"  (TPin)
      );

      return r;
    }
  };


  /*
   * All pins used in this project
   */

  typedef GpioInputPin<GPIOD,0> GpioUartRx;
  typedef GpioOutputPin<GPIOD,1> GpioUartTx;
  typedef GpioOutputPin<GPIOB,0> GpioResetUsb;
  typedef GpioOutputPin<GPIOD,7> GpioResetLtc2986;

  typedef GpioOutputPin<GPIOB,2> GpioSpiCsLtc2986;
  typedef GpioOutputPin<GPIOC,0> GpioSpiCsLed;
  typedef GpioInputPin<GPIOB,4> GpioSpiMiso;
  typedef GpioOutputPin<GPIOB,5> GpioSpiClk;

  struct GpioSpiMosi : GpioOutputPin<GPIOB,3> {

    static void writeByte(uint8_t b) {
      SPDR=b;
      while(!(SPSR & (1 << SPIF)));
    }
  };
}
