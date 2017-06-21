/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Indexes in the EEPROM of the valid mask for a given control. Also the bit-position in
   * the valid nibble.
   */

  enum ValidMask {
    VALID_MASK_HEAT  = 3,
    VALID_MASK_CHILL = 2,
    VALID_MASK_AUX1  = 1,
    VALID_MASK_AUX2  = 0
  };
}
