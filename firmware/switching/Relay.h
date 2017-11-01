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

    protected:
      void activate(uint8_t& currentMask,uint8_t newMask,int8_t action) const;
      int8_t validate(const uint8_t currentMask,uint8_t& newMask);

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

    int8_t action;
    uint8_t newMask;

    action = validate(currentMask,newMask);
    if(action!=-1)
      activate(currentMask,newMask,action);
  }


  /*
   * validate command and return 1 if ON, 0 if OFF, and -1 if fail. If fail then a 
   * response has already been sent
   */

  template<class T,uint8_t TValidMask>
  inline int8_t Relay<T,TValidMask>::validate(const uint8_t currentMask,uint8_t& newMask) {

    // get the parameter. there must be one

    char *parameter=strtok(nullptr," ");
    
    if(parameter==nullptr) {
      Uart::sendString(MissingParameterString,true);
      return -1;
    }

    // check for valid options
    
    if(!strcasecmp(parameter,"ON")) {

      // calculate the proposed new mask

      newMask=currentMask | (1 << TValidMask);

      // check it against the valid mask for the control being activated

      uint8_t validMask=Eeprom::Reader::validMask(TValidMask);
      if((newMask & validMask) != newMask) {
        Uart::sendString(InvalidStateString,true);
        return -1;
      }

      // to protect the relay from crazy software we do not allow switching
      // more often than once every 10 seconds

      if(_lastSwitchingTime==0 || MillisecondTimer::hasTimedOut(_lastSwitchingTime,10000)) {
        _lastSwitchingTime=MillisecondTimer::millis();
        return 1;
      }
      else {
        Uart::sendString(RelayTimeoutString,true);
        return -1;
      }
    }
    else if(!strcasecmp(parameter,"OFF")) {
      
      // turning something off is always permitted

      newMask=currentMask &~ (1 << TValidMask);
      return 0;
    }
    else {
      Uart::sendString(UnknownParameterString,true);
      return -1;
    }
  }


  /*
   * Switch the relay and store the current mask
   */

  template<class T,uint8_t TValidMask>
  inline void Relay<T,TValidMask>::activate(uint8_t& currentMask,uint8_t newMask,int8_t action) const {

    if(action==1)
      T::set();
    else
      T::reset();

    currentMask=newMask;
    Uart::ok();
  }

  // basic relays onboard

  typedef Relay<GpioHeater,ValidMask::VALID_MASK_HEAT> Heater;
  typedef Relay<GpioAux1,ValidMask::VALID_MASK_AUX1> Aux1;
}
