/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#include "Application.h"


/*
 * Timer 1 output compare interrupt handler
 */

ISR(TIMER1_COMPA_vect) {
  brewery::AlarmFlasher::onInterrupt();
}
