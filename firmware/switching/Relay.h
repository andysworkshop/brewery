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

      uint32_t _lastOnTime;
      uint32_t _nextStateTime;

      RelayState _currentState;
      RelayState _nextState;

    protected:
      RelayState parseCommand();
      bool nextStateIsValid(RelayState nextState,uint8_t currentMask,uint8_t& nextMask); 
      uint32_t calculateSwitchingDelay(RelayState nextState);

    public:
      Relay();
      
      void run(uint8_t& currentMask);
      void checkSwitching();
  };


  /*
   * Constructor
   */

  template<class T,uint8_t TValidMask>
  inline Relay<T,TValidMask>::Relay() {
    _lastOnTime=0;
    _nextStateTime=0;
    _currentState=RelayState::OFF;
    _nextState=RelayState::OFF;
  }


  /*
   * Check for the ON/OFF command
   */

  template<class T,uint8_t TValidMask>
  inline void Relay<T,TValidMask>::run(uint8_t& currentMask) {

    // get the proposed next state

    RelayState nextState = parseCommand();
    if(nextState==RelayState::INVALID)
      return;

    // if the desired state is the same as the next state (possibly pending), nothing to do

    if(_nextState==nextState) {
      Uart::ok();
      return;
    }

    // check for invalid state

    uint8_t nextMask;

    if(!nextStateIsValid(nextState,currentMask,nextMask))
      return;

    uint32_t switchingDelay=calculateSwitchingDelay(nextState);

    // set up the time to make the switch

    _nextState=nextState;
    _nextStateTime=MillisecondTimer::millis()+switchingDelay;

    if(switchingDelay) {

      char buffer[100];
      sprintf(buffer,"WARNING:01:Switch scheduled in %ldms",switchingDelay);
      Uart::sendString(buffer,false);
    }
    else
      Uart::ok();
  }


  /*
   * Check for something to do
   */

  template<class T,uint8_t TValidMask>
  inline void Relay<T,TValidMask>::checkSwitching() {

    uint32_t now=MillisecondTimer::millis();

    if(_nextState!=_currentState && now>_nextStateTime) {

      if(_nextState==RelayState::ON) {
        _lastOnTime=now;
        T::set();
      }
      else {
        T::reset();
      }

      _currentState=_nextState;
    }
  }


  /*
   * Parse the command (ON/OFF)
   */

  template<class T,uint8_t TValidMask>
  inline RelayState Relay<T,TValidMask>::parseCommand() {

    // get the parameter. there must be one

    char *parameter=strtok(nullptr," ");
    
    if(parameter==nullptr) {
      Uart::sendString(MissingParameterString,true);
      return RelayState::INVALID;
    }

    // check for valid options
    
    if(!strcasecmp(parameter,"ON"))
      return RelayState::ON;
    else if(!strcasecmp(parameter,"OFF"))
      return RelayState::OFF;

    // invalid

    Uart::sendString(UnknownParameterString,true);
    return RelayState::INVALID;
  }


  /*
   * Check if the desired state is permitted (e.g. CHILL not at same time as HEAT)
   */

  template<class T,uint8_t TValidMask>
  inline bool Relay<T,TValidMask>::nextStateIsValid(RelayState nextState,uint8_t currentMask,uint8_t& nextMask) {

    if(nextState==RelayState::ON) {

      // calculate the proposed new mask

      nextMask=currentMask | (1 << TValidMask);

      // check it against the valid mask for the control being activated

      uint8_t validMask=Eeprom::Reader::validMask(TValidMask);
      if((nextMask & validMask) != nextMask) {
        Uart::sendString(InvalidStateString,true);
        return false;
      }

      return true;
    }
    else {
      
      // turning something off is always permitted

      nextMask=currentMask &~ (1 << TValidMask);
      return true;
    }
  }


  /*
   * Calculate the switching delay, if any
   */

  template<class T,uint8_t TValidMask>
  inline uint32_t Relay<T,TValidMask>::calculateSwitchingDelay(RelayState nextState) {
  
    // always all OFF

    if(nextState==RelayState::OFF || _lastOnTime==0)
      return 0;

    uint32_t difference=MillisecondTimer::difference(_lastOnTime);

    // if we switched on less than 10 seconds ago then we need to pause
    // if there's a custom blackout in effect then factor it in
    
    uint32_t customPeriod = T::getBlackoutPeriod();
    uint32_t blackout = customPeriod>10000 ? customPeriod : 10000;

    if(difference<blackout)
      return blackout-difference;

    return 0;
  }


  // basic relays onboard

  typedef Relay<GpioHeater,ValidMask::VALID_MASK_HEAT> Heater;
  typedef Relay<GpioChiller,ValidMask::VALID_MASK_CHILL> Chiller;
  typedef Relay<GpioAux1,ValidMask::VALID_MASK_AUX1> Aux1;
}
