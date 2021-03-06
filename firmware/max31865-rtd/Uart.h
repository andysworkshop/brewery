/*
 * Andy's Workshop Brewery Max31865 RTD controller ATMega328p firmware
 * Copyright (c) 2018 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * The UART peripheral class handles sending and receiving data. Data is transmitted
   * and received asynchronously.
   */

  class Uart {

    public:
      enum {
        MAX_COMMAND_SIZE = 100   // limit to 8 bit max size
      };

    protected:
      volatile static bool _commandReady;
      volatile static char _receiveBuffer[MAX_COMMAND_SIZE];
      uint8_t static _receiveBufferPos;              // next position to store data

    protected:
      static void sendChecksum(const char *buffer,bool progmem);

    public:
      static void setup();
      static void sendString(const char *buffer,bool progmem);

      static void onReceiveInterrupt(char c);

      static bool isCommandReady();
      static char *getCommandPtr();

      static void ok();
  };


  /*
   * setup the pins
   */

  inline void Uart::setup() {

    UBRR0 = 51;                               // 9600 bps
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);    // enable TX/RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   // 8-N-1

    // enable receive interrupts

    UCSR0B |= (1 << RXCIE0);
  }


  /*
   * send a string, optionally from PROGMEM. The client will be waiting for this response
   * so it's send synchronously because nothing else will be going on. That means the
   * caller can pass buffer on the stack if necessary.
   */

  inline void Uart::sendString(const char *buffer,bool progmem) {

    // turnaround delay - the PC is not a realtime device
    // and this is asynchronous communication

    MillisecondTimer::delay(500);

    // first send the 16-bit checksum

    sendChecksum(buffer,progmem);

    // send the data

    char c;

    do {

      if(progmem)
        c=pgm_read_byte_near(buffer);
      else
        c=*buffer;

      if(c) {
        while(!(UCSR0A & (1<<UDRE0)));
        UDR0=c;
        buffer++;
      }

    } while(c);

    // send a CRLF pair to finish
    
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0='\r';
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0='\n';

    // ready to receive again

    _receiveBufferPos=0;
    _commandReady=false;
  }


  /**
   * Send the 16-bit checksum as ASCII followed by ":"
   */

  inline void Uart::sendChecksum(const char *buffer,bool progmem) {

    // do the calculation

    uint8_t b;
    uint16_t crc;

    crc=0;

    do {

      if(progmem)
        b=pgm_read_byte_near(buffer);
      else
        b=*buffer;

      if(b) {
        crc=_crc16_update(crc,b);
        buffer++;
      }

    } while(b);

    char output[20];
    sprintf(output,"%u:",crc);

    for(char *ptr=output;*ptr;ptr++) {
      while(!(UCSR0A & (1<<UDRE0)));
      UDR0=*ptr;
    }
  }


  /*
   * A character was received
   */

  inline void Uart::onReceiveInterrupt(char c) {

    // if there's space in the buffer and we're not out-of-sync
    // then receive the byte

    if(_receiveBufferPos<sizeof(_receiveBuffer) && !_commandReady) {

      if(c=='\n') {

        // get rid of any CR preceding the LF or just the LF itself

        if(_receiveBufferPos && _receiveBuffer[_receiveBufferPos-1]=='\r')
          _receiveBuffer[_receiveBufferPos-1]='\0';
        else
          _receiveBuffer[_receiveBufferPos]='\0';

        // signal that a command is ready

        _commandReady=true;
      }
      else {

        // ~ is the reset character. If we see this then the buffer is reset
        // to empty.

        if(c=='~') {
          _receiveBufferPos=0;
        }
        else 
          _receiveBuffer[_receiveBufferPos++]=c;
      }
    }
  }
  
  
  /*
   * return true if a command is ready for processing
   */

  inline bool Uart::isCommandReady() {
    return _commandReady;
  }


  /*
   * Get a pointer to the command
   */

  inline char *Uart::getCommandPtr() {
    return const_cast<char *>(_receiveBuffer);
  }


  /*
   * Send an OK response
   */

  inline void Uart::ok() {
    sendString(OKString,true);
  }
}
