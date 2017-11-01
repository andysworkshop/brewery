/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#include "Application.h"


namespace brewery {

  // static definitions

  volatile bool Uart::_commandReady=false;
  uint8_t Uart::_receiveBufferPos=0;
  volatile char Uart::_receiveBuffer[Uart::MAX_COMMAND_SIZE];
}


/*
 * RX interrupt vector
 */

ISR(USART_RX_vect) {
  char c=UDR0;
  brewery::Uart::onReceiveInterrupt(c);
}
