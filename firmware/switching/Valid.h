/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /**
   * Each of the four controls has a 4-bit 'valid mask' that has a '1' in the position where a control may
   * be on at the same time as another. A request to switch on a control should calculate the new 4-bit
   * state then AND it against the valid mask for the control that is requesting to be switched on. The
   * control must ONLY be switched on if the AND result is the same as the proposed new state.
   *
   * The 4-bit mask is:
   *
   *    3     2      1     0
   *    HEAT  CHILL  AUX1  AUX2
   * 
   * The default masks permit no sharing at all. That is, only one control may be active at a time.
   */ 

  struct Valid {
    static void run();
  };


  /*
   * Process the VALID command. The format is VALID <heat> <chill> <aux1> <aux2>
   */

  inline void Valid::run() {

    // read the masks as plain integers. don't write to EEPROM until we've got
    // a valid full set.

    uint8_t masks[4];

    for(uint8_t i=0;i<=3;i++) {
  
      char *parameter=strtok(nullptr," ");
    
      if(parameter==nullptr) {
        Uart::sendString(MissingParameterString,true);
        return;
      }

      // the parameter is a percentage

      masks[i]=atoi(parameter);
    }

    // write to EEPROM

    Eeprom::Writer::validMask(ValidMask::VALID_MASK_HEAT,masks[0]);
    Eeprom::Writer::validMask(ValidMask::VALID_MASK_CHILL,masks[1]);
    Eeprom::Writer::validMask(ValidMask::VALID_MASK_AUX1,masks[2]);
    Eeprom::Writer::validMask(ValidMask::VALID_MASK_AUX2,masks[3]);

    // all good
    
    Uart::ok();
  }
}
