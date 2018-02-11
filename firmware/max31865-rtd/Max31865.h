/*
 * Andy's Workshop Brewery MAX31865 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once

namespace brewery {

    namespace Register {
      static constexpr uint8_t CONFIG    = 0x00;
      static constexpr uint8_t RTDMSB    = 0x01;
      static constexpr uint8_t RTDLSB    = 0x02;
      static constexpr uint8_t HFAULTMSB = 0x03;
      static constexpr uint8_t HFAULTLSB = 0x04;
      static constexpr uint8_t LFAULTMSB = 0x05;
      static constexpr uint8_t LFAULTLSB = 0x06;
      static constexpr uint8_t FAULTSTAT = 0x07;
    }

    namespace ConfigBits {
      static constexpr uint8_t BIAS       = 0x80;
      static constexpr uint8_t MODEAUTO   = 0x40;
      static constexpr uint8_t MODEOFF    = 0x00;
      static constexpr uint8_t SHOT1      = 0x20;
      static constexpr uint8_t WIRE3      = 0x10;
      static constexpr uint8_t WIRE24     = 0x00;
      static constexpr uint8_t FAULTDETHI = 0x08;
      static constexpr uint8_t FAULTDETLO = 0x04;
      static constexpr uint8_t FAULTSTAT  = 0x02;
      static constexpr uint8_t FILT50HZ   = 0x01;
      static constexpr uint8_t FILT60HZ   = 0x00;
    }

    namespace FaultBits {
      static constexpr uint8_t HIGHTHRESH = 0x80;
      static constexpr uint8_t LOWTHRESH  = 0x40;
      static constexpr uint8_t REFINLOW   = 0x20;
      static constexpr uint8_t REFINHIGH  = 0x10;
      static constexpr uint8_t RTDINLOW   = 0x08;
      static constexpr uint8_t OVUV       = 0x04;
    }

  /*
   * MAX3185 implementation
   */

  template<class TGpioCS, class TGpioDRDY>
  class Max31865 : public Max31865Base {

    protected:
      static void clearFault();
      static void enableBias(bool b);
      static double temperature(float RTDnominal, float refResistor);
      static uint16_t readRTD();

      static void writeByte(uint8_t reg, uint8_t bits);
      static uint8_t readByte(uint8_t reg);
      static uint8_t readFault();
      static uint16_t readWord(uint8_t reg);

    public:
      static void setup();
      static void run(RtdReadings::Result& reading);
  };


  template<class TGpioCS, class TGpioDRDY>
  inline void Max31865<TGpioCS, TGpioDRDY>::setup() {
    
    writeByte(Register::CONFIG, ConfigBits::WIRE3 | ConfigBits::FILT50HZ);    
    clearFault();
  }


  /*
   * Take a reading
   */

  template<class TGpioCS, class TGpioDRDY>
  inline void Max31865<TGpioCS, TGpioDRDY>::run(RtdReadings::Result& reading) {

    reading.faultCode = readFault();
  
    if(reading.faultCode==0)
      reading.temperature = getTemperature(readRTD(), 100, 400);
    else
      reading.temperature = 0;
  }


  template<class TGpioCS, class TGpioDRDY>
  inline void Max31865<TGpioCS, TGpioDRDY>::clearFault() {
    
    uint8_t reg = readByte(Register::CONFIG);
    
    reg &= ~0x2C;
    reg |= ConfigBits::FAULTSTAT;
    writeByte(Register::CONFIG, reg);
  }


  template<class TGpioCS, class TGpioDRDY>
  inline uint8_t Max31865<TGpioCS, TGpioDRDY>::readFault() {
    return readByte(Register::FAULTSTAT);    
  }


  template<class TGpioCS, class TGpioDRDY>
  inline void Max31865<TGpioCS, TGpioDRDY>::enableBias(bool b) {
  
    uint8_t t = readByte(Register::CONFIG);
  
    if (b)
      t |= ConfigBits::BIAS;
    else
      t &= ~ConfigBits::BIAS;

    writeByte(Register::CONFIG, t);
  }


  template<class TGpioCS, class TGpioDRDY>
  uint16_t Max31865<TGpioCS, TGpioDRDY>::readRTD() {

    GpioAlarm::set();

    clearFault();
    enableBias(true);
    MillisecondTimer::delay(10);
    
    uint8_t t = readByte(Register::CONFIG);
    t |= ConfigBits::SHOT1;      
    writeByte(Register::CONFIG, t);

    // max time at 50Hz = 66ms

    MillisecondTimer::delay(70);

    uint16_t rtd = readWord(Register::RTDMSB);

    // fault detection cycle

    t = readByte(Register::CONFIG);
    t |= ConfigBits::FAULTDETHI;   
    writeByte(Register::CONFIG, t);
    MillisecondTimer::delay(10);
    t = readByte(Register::CONFIG);
    t |= ConfigBits::FAULTDETHI | ConfigBits::FAULTDETLO;   
    writeByte(Register::CONFIG, t);

    while((readByte(Register::CONFIG) & (ConfigBits::FAULTDETHI | ConfigBits::FAULTDETLO))!=0)
      MillisecondTimer::delay(2);

    enableBias(false);

    GpioAlarm::reset();

    // remove fault

    rtd >>= 1;
    return rtd;
  }


  template<class TGpioCS, class TGpioDRDY>
  void Max31865<TGpioCS, TGpioDRDY>::writeByte(uint8_t reg, uint8_t bits) {
    
    TGpioCS::reset();
    GpioSpiMosi::writeByte(reg | 0x80);
    GpioSpiMosi::writeByte(bits);
    TGpioCS::set();
  }


  template<class TGpioCS, class TGpioDRDY>
  uint8_t Max31865<TGpioCS, TGpioDRDY>::readByte(uint8_t reg) {

    TGpioCS::reset();
    GpioSpiMosi::writeByte(reg);
    uint8_t value = GpioSpiMosi::writeByte(0xff);
    TGpioCS::set();

    return value;
  }
  

  template<class TGpioCS, class TGpioDRDY>
  uint16_t Max31865<TGpioCS, TGpioDRDY>::readWord(uint8_t reg) {

    TGpioCS::reset();
    GpioSpiMosi::writeByte(reg);
    
    uint16_t hi = GpioSpiMosi::writeByte(0xff);
    uint16_t lo = GpioSpiMosi::writeByte(0xff);

    TGpioCS::set();

    return (hi << 8) | lo;
  }
}
