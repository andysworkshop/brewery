/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#include "Application.h"


namespace brewery {
  volatile uint32_t MillisecondTimer::_counter=0;
  volatile uint8_t MillisecondTimer::_subCounter=0;
}


/*
 * Timer 0 interrupt handler
 */

ISR(TIMER0_OVF_vect) {

  using namespace brewery;

  MillisecondTimer::_subCounter++;

  if((MillisecondTimer::_subCounter & 0x3)==0)
    MillisecondTimer::_counter++;
  
  TCNT0+=6;
}
