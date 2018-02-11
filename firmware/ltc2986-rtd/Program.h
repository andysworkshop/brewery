/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Main program class
   */

  class Program {
    
    protected:
      RtdReadings _lastReadings;
      uint32_t _lastReadingsTime;

    public:
      Program();
      void run();
  };


  /*
   * Constructor
   */

  inline Program::Program() {
   
    _lastReadingsTime=0;

    // check the EEPROM

    Eeprom::verifyState();

    // setup peripherals

    GpioResetLtc2986::setup();
    LTC2986::setup();
    Uart::setup();

    // setup the SPI pins

    GpioSpiCsLtc2986::setup();
    GpioSpiCsLtc2986::set();

    GpioSpiCsLed::setup();
    GpioSpiCsLed::set();

    GpioSpiMiso::setup();
    GpioSpiMosi::setup();
    GpioSpiClk::setup();

    // set up the alarm

    GpioAlarm::setup();
    AlarmFlasher::setup();
    
    // restore from whatever the previous state was
    
    Alarm::setLed();

    // initialisers

    MillisecondTimer::setup();
    Max7221::setup();
  }  


  /*
   * Run this program and don't return
   */

  inline void Program::run() {

    // enable global interrupt

    sei();

    // reset the LTC2986

    GpioResetLtc2986::reset();
    MillisecondTimer::delay(100);
    GpioResetLtc2986::set();

    // the LTC2986 has a 200ms startup delay

    MillisecondTimer::delay(250);

    // configure the LTC2986

    LTC2986::configure();

    // enable watchdog on a 2 second timer

    wdt_enable(WDTO_2S);

    for(;;) {

      // take sensor readings

      if(_lastReadingsTime==0 || MillisecondTimer::hasTimedOut(_lastReadingsTime,1000)) {
        LTC2986::run(_lastReadings);
        _lastReadingsTime=MillisecondTimer::millis();
        _lastReadings.display();
      }

      // check for commands to process

      CommandProcessor::run(_lastReadings);

      // reset the watchdog

      wdt_reset();
    }
  }
}
