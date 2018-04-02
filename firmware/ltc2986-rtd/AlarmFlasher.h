/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {


  /*
   * Class to flash the alarm LED at 2Hz. Uses the 16-bit timer1 ticking at 128uS
   * with an output compare interrupt handler at 3906 ticks
   */

  class AlarmFlasher {

    public:
      static void setup();

      static void on();
      static void off();

      static void onInterrupt();
  };


  /*
   * Setup timer1
   */

  inline void AlarmFlasher::setup() {

    // these start off at zero but are set here for clarity

    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TCNT1 = 0;

    // output compare = 3906. 128uS x 3906 = 0.5s (2Hz)
    
    OCR1A = 3906;

    // enable interrupts

    TIMSK1 = _BV(OCIE1A);
  }

  
  /*
   * Enable by setting the clock source to clk/1024 = 128uS
   */

  inline void AlarmFlasher::on() {
    TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12);
  }


  /*
   * Disable by disconnecting from the clock source
   */

  inline void AlarmFlasher::off() {
    TCCR1B = 0;
  }

  
  /*
   * Interrupt handler, toggle the pin
   */

  inline void AlarmFlasher::onInterrupt() {
    GpioAlarm::changeTo(GpioAlarm::read() ^ true);
  }
}

