/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /**
   * Current state of the alarm LED
   */
  
  enum AlarmState {
    OFF,
    ON,
    FLASH
  };
}
