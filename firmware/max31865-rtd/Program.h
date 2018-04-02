/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
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

    Uart::setup();

    // setup the GPIO pins

    GpioMaxDataReadyBlue::setup();
    GpioMaxDataReadyRed::setup();
    GpioMaxCsRed::setup();
    GpioMaxCsBlue::setup();

    GpioMaxCsRed::set();
    GpioMaxCsBlue::set();

    GpioEnableLed::setup();
    GpioEnableLed::set();

    GpioSpiCsLed::setup();
    GpioSpiCsLed::set();

    GpioSpiMiso::setup();
    GpioSpiMosi::setup();
    GpioSpiClk::setup();
    
    // standard SPI gotcha. SS is NC on this board but must be output high or
    // the SPI peripheral will switch to slave mode

    GpioSpiSs::setup();
    GpioSpiSs::set();

    // set up the alarm

    GpioAlarm::setup();
    AlarmFlasher::setup();

    // setup the timer

    MillisecondTimer::setup();

    // restore from whatever the previous state was
    
    Alarm::setLed();

    // SPI setup
    // Selected clock is osc/8 = 1MHz, mode is 0

    SPCR=(1 << MSTR) |                  // master
         (1 << SPE) |                   // enabled
         (1 << CPHA) |                  // CPOL = 1, CPHA = 1 (SPI mode 3)
         (1 << CPOL) |
         (0 << SPR1) | (1 << SPR0);     // fosc/16

    SPSR |= (1 << SPI2X);               // fosc/16 * 2 = fosc/8 = 1Mhz

    // enable global interrupt

    sei();

    // initialisers

    Max7221::setup();

    Max31865<GpioMaxCsRed,GpioMaxDataReadyRed>::setup();
    Max31865<GpioMaxCsBlue,GpioMaxDataReadyBlue>::setup();
  }  


  /*
   * Run this program and don't return
   */

  inline void Program::run() {

    // enable watchdog on a 2 second timer

    wdt_enable(WDTO_2S);

    for(;;) {

      // take sensor readings

      if(_lastReadingsTime==0 || MillisecondTimer::hasTimedOut(_lastReadingsTime,1000)) {
        
        Max31865<GpioMaxCsBlue,GpioMaxDataReadyBlue>::run(_lastReadings.rtd1);
        Max31865<GpioMaxCsRed,GpioMaxDataReadyRed>::run(_lastReadings.rtd2);

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
