/*
 * Andy's Workshop Brewery MAX31865 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7


namespace brewery {

  /*
   * MAX3185 base implementation
   */

  class Max31865Base {
    protected:
      static double getTemperature(double Rt, double RTDnominal, double refResistor);
  };


  /*
   * Convert the Rt register reading into a temperature
   */

  inline double Max31865Base::getTemperature(double Rt, double RTDnominal, double refResistor) {

    double Z1, Z2, Z3, Z4, temp;

    Rt /= 32768;
    Rt *= refResistor;
    
    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / RTDnominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;
    
    if (temp >= 0) 
      return temp;

    // ugh.
    float rpoly = Rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= Rt;  // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt;  // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt;  // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt;  // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
  }
}
