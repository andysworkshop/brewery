/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Generic relay implementation TValidMask is one of the ValidMask enumeration values
   */
   
  class Chiller : public Relay<GpioChiller,ValidMask::VALID_MASK_CHILL> {

    protected:
      uint32_t _lastOffTime;
      uint32_t _blackoutTime;

    public:
      Chiller();
      void run(uint8_t& currentMask);     
  };


  /*
   * Constructor
   */

  inline Chiller::Chiller() {

    _lastOffTime=0;
    _blackoutTime=Eeprom::Reader::chillBlackout();
  }


  /*
   * Run the command
   */

  inline void Chiller::run(uint8_t& currentMask) {

    // call superclass validation

    int8_t action;
    uint8_t newMask;

    action = validate(currentMask,newMask);
    if(action==-1)
      return;

    // local validation

    if(action==1 && _lastOffTime) {

      // want to switch on and we have remembered the last off time

      if(MillisecondTimer::hasTimedOut(_lastOffTime,_blackoutTime)) {
        Uart::sendString(ChillerBlackoutString,true);
        return;
      }
    }

    // take action

    activate(currentMask,newMask,action);

    // remember last off time

    if(action==0)
      _lastOffTime=MillisecondTimer::millis();
  }
}

