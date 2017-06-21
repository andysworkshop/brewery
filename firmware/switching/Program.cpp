/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

 #include "Application.h"


/*
 * Main entry point. Note that the fuse settings for the application
 * should be set for an external 8MHz oscillator.
 */

int main() {

  using namespace brewery;
  
  // disable watchdog in case we got here from a software reset

  MCUSR = 0;
  wdt_disable();

  // interrupts off

  cli();

  // setup peripherals

  Uart::setup();

  GpioHeater::setup();
  GpioChiller::setup();
  GpioAux1::setup();
  GpioAux2::setup();

  // zero crossing sense pin

  GpioZeroSense::setup(); 

  // initialisers

  MillisecondTimer::setup();
  TriacControl::setup();
  
  // declare the program class

  brewery::Program p;
  p.run(); 
 
  // not reached

  return 0;
}
