/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {


  /*
   * Management class for EEPROM access
   */

  struct Eeprom {

    /*
     * EEPROM locations
     */

    enum Location : uint8_t {
      MAGIC       = 0,     // 2 byte signature
      VALID_MASKS = 2,     // 4 bytes
      SERIAL      = 6      // 4 bytes (BOARD_SERIAL macro)
    };

    
    /*
     * The magic number that indicates valid EEPROM content
     */

    enum {
      MAGIC_NUMBER = 0x55aa
    };


    static void verifyState();
    static void writeDefaults();

    /*
     * High level read access
     */

    class Reader {
      
      public:
        static uint16_t magic();
        static uint8_t readByte(uint8_t l);
        static uint32_t readUint32(Location l);

        static uint8_t validMask(uint8_t index);
        static uint32_t serial();
    };


    /*
     * High level write access
     */

    struct Writer {
      static void magic();
      static void writeUint32(Location l,uint32_t u);
      static void writeByte(uint8_t l,uint8_t b);

      static void validMask(uint8_t index,uint8_t b);
      static void serial();
    };
  };


  /*
   * Read a byte
   */

  inline uint8_t Eeprom::Reader::readByte(uint8_t l) {
    
    // read and return the byte

    return eeprom_read_byte(reinterpret_cast<uint8_t *>(l));
  }


  /*
   * Read a uint32_t
   */

  inline uint32_t Eeprom::Reader::readUint32(Location l) {
    return eeprom_read_dword(reinterpret_cast<uint32_t *>(l));
  }


  /*
   * Read the magic number
   */

  inline uint16_t Eeprom::Reader::magic() {
    return eeprom_read_word(reinterpret_cast<uint16_t *>(Location::MAGIC));
  }


  /*
   * Read the valid mask for a control
   */

  inline uint8_t Eeprom::Reader::validMask(uint8_t index) {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(Location::VALID_MASKS+index));
  }


  /*
   * Read the serial number
   */

  inline uint32_t Eeprom::Reader::serial() {
    return readUint32(Location::SERIAL);
  }


  /*
   * Write a byte to the location
   */

  inline void Eeprom::Writer::writeByte(uint8_t l,uint8_t b) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(l),b);
  }


  /*
   * Write a uint32_t to the location
   */

  inline void Eeprom::Writer::writeUint32(Location l,uint32_t u) {
    eeprom_write_dword(reinterpret_cast<uint32_t *>(l),u);
  }


  /*
   * Write the magic number
   */

  inline void Eeprom::Writer::magic() {
    eeprom_write_word(reinterpret_cast<uint16_t *>(Location::MAGIC),MAGIC_NUMBER);
  }


  /*
   * Write the valid mask for a control
   */

  inline void Eeprom::Writer::validMask(uint8_t index,uint8_t b) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(Location::VALID_MASKS+index),b);
  }


  /*
   * Write the board serial number
   */

  inline void Eeprom::Writer::serial() {
    writeUint32(Location::SERIAL,BOARD_SERIAL);
  }


  /*
   * Verify the content and default it if invalid
   */

  inline void Eeprom::verifyState() {
   
    // check for the signature and return if OK

    if(Reader::magic()==MAGIC_NUMBER)
      return;

    writeDefaults();
  }


  /*
   * Write the default values to EEPROM
   */

  inline void Eeprom::writeDefaults() {

    // write out default content

    Writer::validMask(ValidMask::VALID_MASK_HEAT,0b1000);
    Writer::validMask(ValidMask::VALID_MASK_CHILL,0b0100);
    Writer::validMask(ValidMask::VALID_MASK_AUX1,0b0010);
    Writer::validMask(ValidMask::VALID_MASK_AUX2,0b0001);

    Writer::serial();
    Writer::magic();
  }
}
