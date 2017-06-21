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
   
  template<class T,uint8_t TValidMask>
  class Relay {

    public:
      uint32_t _lastSwitchingTime;

    public:
      Relay();
      void run(uint8_t& currentMask);
  };


  /*
   * Constructor
   */

  template<class T,uint8_t TValidMask>
  inline Relay<T,TValidMask>::Relay() {
    _lastSwitchingTime=0;
  }


  /*
   * Check for the ON/OFF command
   */

  template<class T,uint8_t TValidMask>
  inline void Relay<T,TValidMask>::run(uint8_t& currentMask) {

    // get the parameter. there must be one

    char *parameter=strtok(nullptr," ");
    
    if(parameter==nullptr) {
      Uart::sendString(MissingParameterString,true);
      return;
    }

    // check for valid options
    
    uint8_t newMask;

    if(!strcasecmp(parameter,"ON")) {

      // calculate the proposed new mask

      newMask=currentMask | (1 << TValidMask);

      // check it against the valid mask for the control being activated

      uint8_t validMask=Eeprom::Reader::validMask(TValidMask);
      if((newMask & validMask) != newMask) {
        Uart::sendString(InvalidStateString,true);
        return;
      }

      // to protect the relay from crazy software we do not allow switching
      // more often than once every 10 seconds

      if(_lastSwitchingTime==0 || MillisecondTimer::hasTimedOut(_lastSwitchingTime,10000)) {
        T::set();
        Uart::ok();
        _lastSwitchingTime=MillisecondTimer::millis();
      }
      else {
        Uart::sendString(RelayTimeoutString,true);
        return;
      }
    }
    else if(!strcasecmp(parameter,"OFF")) {
      
      // turning something off is always permitted

      newMask=currentMask &~ (1 << TValidMask);

      T::reset();
      Uart::ok();
    }
    else {
      Uart::sendString(UnknownParameterString,true);
      return;
    }

    currentMask=newMask;
  }


  // relays onboard

  typedef Relay<GpioHeater,ValidMask::VALID_MASK_HEAT> Heater;
  typedef Relay<GpioChiller,ValidMask::VALID_MASK_CHILL> Chiller;
  typedef Relay<GpioAux1,ValidMask::VALID_MASK_AUX1> Aux1;
}
