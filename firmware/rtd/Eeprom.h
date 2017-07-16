/*
 * Andy's Workshop NXA66 controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * General utilities to read/write configuration values to the EEPROM. Functionality
   * is divided between Reader and Writer classes.
   */
   
  struct Eeprom {

    /*
     * EEPROM locations
     */

    enum class Location : uint8_t {
      MAGIC       = 0,      // 2 byte signature
      INTENSITY   = 2,      // 1 byte (8)
      DISPLAYS    = 3       // 1 byte (RED | BLUE)
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
      
      protected:
        static uint16_t validate(uint16_t value);

      public:
        static uint16_t magic();
        static uint16_t constant(Location l);
        static uint8_t readByte(Location l);
        static uint8_t intensity();
        static uint8_t displays();
    };

    /*
     * High level write access
     */

    struct Writer {
      static void magic();
      static void constant(Location l,uint16_t c);
      static void writeByte(Location l,uint8_t b);
      static void intensity(uint8_t i);
      static void displays(uint8_t displays);
    };
  };


  /*
   * Read a byte
   */

  inline uint8_t Eeprom::Reader::readByte(Location l) {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(l));
  }


  /*
   * Read a constant value
   */

  inline uint16_t Eeprom::Reader::constant(Location l) {
    return eeprom_read_word(reinterpret_cast<uint16_t *>(l));
  }


  /*
   * Read the magic number
   */

  inline uint16_t Eeprom::Reader::magic() {
    return eeprom_read_word(reinterpret_cast<uint16_t *>(Location::MAGIC));
  }


  /*
   * Read the LCD intensity value (0..15)
   */

  inline uint8_t Eeprom::Reader::intensity() {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(Location::INTENSITY));
  }


  /*
   * Read the displays to use
   */

  inline uint8_t Eeprom::Reader::displays() {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(Location::DISPLAYS));
  }


  /*
   * Write a byte to the location
   */

  inline void Eeprom::Writer::writeByte(Location l,uint8_t b) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(l),b);
  }


  /*
   * Write a constant value
   */

  inline void Eeprom::Writer::constant(Location l,uint16_t c) {
    eeprom_write_word(reinterpret_cast<uint16_t *>(l),c);
  }


  /*
   * Write the magic number
   */

  inline void Eeprom::Writer::magic() {
    eeprom_write_word(reinterpret_cast<uint16_t *>(Location::MAGIC),MAGIC_NUMBER);
  }


  /*
   * Write the intensity
   */

  inline void Eeprom::Writer::intensity(uint8_t i) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(Location::INTENSITY),i);
  }


  /*
   * Write the displays to use
   */

  inline void Eeprom::Writer::displays(uint8_t displays) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(Location::DISPLAYS),displays);
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

    Writer::intensity(12);
    Writer::displays(0x3);

    Writer::magic();
  }
}

