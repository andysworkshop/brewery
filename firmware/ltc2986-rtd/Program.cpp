/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
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

  // declare the program class

  brewery::Program p;
  p.run(); 
 
  // not reached

  return 0;
}
