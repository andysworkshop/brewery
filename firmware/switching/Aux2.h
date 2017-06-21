/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Aux2 takes care of the Triac output
   */

  struct Aux2 {
    static void run(uint8_t& currentMask);
  };


  /*
   * Process the AUX2 <percentage> command
   */

  inline void Aux2::run(uint8_t& currentMask) {

    // get the parameter. there must be one

    char *parameter=strtok(nullptr," ");
    
    if(parameter==nullptr) {
      Uart::sendString(MissingParameterString,true);
      return;
    }

    // the parameter is a percentage

    uint8_t value=atoi(parameter);
    
    if(value>100) {
      Uart::sendString(OutOfRangeString,true);
      return;
    }

    uint8_t newMask;

    if(value!=0) {
          
      // calculate the proposed new mask

      newMask=currentMask | (1 << ValidMask::VALID_MASK_AUX2);

      // check it against the valid mask for AUX2

      uint8_t validMask=Eeprom::Reader::validMask(ValidMask::VALID_MASK_AUX2);
      if((newMask & validMask) != newMask) {
        Uart::sendString(InvalidStateString,true);
        return;
      }
    }
    else
      newMask=currentMask &~ (1 << ValidMask::VALID_MASK_AUX2);

    // set the new duty cycle

    TriacControl::setDutyCycle(value);
    Uart::ok();

    currentMask=newMask;
  }
}
