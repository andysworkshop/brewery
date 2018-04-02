/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
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
