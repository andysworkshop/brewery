/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {


  /*
   * Controller class for the LTC2986 device
   */

  class LTC2986 {

    protected:
      static void configureChannels();
      static void configureGlobal();
      static void assignChannel(uint8_t channelNumber,uint32_t channelAssignmentData);
      static uint32_t transferFourBytes(uint8_t rw,uint16_t startAddress,uint32_t inputData);
      static uint8_t transferByte(uint8_t rw,uint16_t startAddress,uint8_t inputData);
      static void spiTransferBlock(const uint8_t *tx,uint8_t *rx,uint8_t length);

      static void measureChannel(uint8_t channelNumber,uint8_t channelOutput,RtdReadings::Result& result);
      static void convertChannel(uint8_t channelNumber);
      static void waitForProcessToFinish();
      static void getResult(uint8_t channelNumber,uint8_t channelOutput,RtdReadings::Result& result);
      static void setConversionResult(uint32_t raw_conversion_result,uint8_t channelOutput,RtdReadings::Result& result);
      static uint16_t getStartAddress(uint16_t base_address, uint8_t channel_number);

    public:
      static void setup();
      static void configure();
      static void run(RtdReadings& results);
  };


  /*
   * Setup the device
   */

  inline void LTC2986::setup() {

    // Max LTC2986 clock = 2MHz
    // Selected clock is osc/8 = 1MHz, mode is 0

    SPCR=(1 << MSTR) |                  // master
         (1 << SPE) |                   // enabled
         (0 << SPR1) | (1 << SPR0);     // fosc/16

    SPSR |= (1 << SPI2X);               // fosc/16 * 2 = fosc/8 = 1Mhz
  }


  /*
   * Configure the device
   */

  inline void LTC2986::configure() {
    configureChannels();
    configureGlobal();
  }


  /*
   * Configure global parameters
   */

  inline void LTC2986::configureGlobal() {
   
    // -- Set global parameters
   
    transferByte(
      WRITE_TO_RAM, 
      0xF0, 
      TEMP_UNIT__C | REJECTION__50_60_HZ | ENABLE_KELVIN_3_WIRE_RTD_MODE);
    
    // -- Set any extra delay between conversions (in this case, 0*100us)
    
    transferByte(WRITE_TO_RAM, 0xFF, 255);
  }


  /*
   * Configure the channels
   */

  inline void LTC2986::configureChannels() {

    uint32_t channelAssignmentData;

    // ----- Channel 2: Assign Sense Resistor -----
    
    channelAssignmentData = 
      SENSOR_TYPE__SENSE_RESISTOR |
      (uint32_t) 0xFA000 << SENSE_RESISTOR_VALUE_LSB;   // sense resistor - value: 1000.

    assignChannel(2,channelAssignmentData);
    
    // ----- Channel 4: Assign RTD PT-100 -----
    
    channelAssignmentData = 
      SENSOR_TYPE__RTD_PT_100 |
      RTD_RSENSE_CHANNEL__2 |
      RTD_NUM_WIRES__3_WIRE |
      RTD_EXCITATION_MODE__NO_ROTATION_SHARING |
      RTD_EXCITATION_CURRENT__500UA |
      RTD_STANDARD__EUROPEAN;
    
    assignChannel(4,channelAssignmentData);
    
    // ----- Channel 8: Assign RTD PT-100 -----
    
    channelAssignmentData = 
      SENSOR_TYPE__RTD_PT_100 |
      RTD_RSENSE_CHANNEL__2 |
      RTD_NUM_WIRES__3_WIRE |
      RTD_EXCITATION_MODE__NO_ROTATION_SHARING |
      RTD_EXCITATION_CURRENT__500UA |
      RTD_STANDARD__EUROPEAN;
    
    assignChannel(8,channelAssignmentData);
  }


  /*
   * Assign a channel
   */

  inline void LTC2986::assignChannel(uint8_t channelNumber,uint32_t channelAssignmentData) {
    uint16_t startAddress = getStartAddress(CH_ADDRESS_BASE,channelNumber);
    transferFourBytes(WRITE_TO_RAM,startAddress, channelAssignmentData);
  }


  /*
   * Transfer four bytes to the device
   */

  inline uint32_t LTC2986::transferFourBytes(uint8_t rw,uint16_t startAddress,uint32_t inputData) {

    uint32_t outputData;
    uint8_t tx[7],rx[7];

    tx[6]=rw;
    tx[5]=startAddress >> 8;
    tx[4]=startAddress;
    tx[3]=inputData >> 24;
    tx[2]=inputData >> 16;
    tx[1]=inputData >> 8;
    tx[0]=inputData;

    spiTransferBlock(tx,rx,7);

    outputData = (uint32_t) rx[3] << 24 |
                  (uint32_t) rx[2] << 16 |
                  (uint32_t) rx[1] << 8  |
                  (uint32_t) rx[0];

    return outputData;
  }


  /*
   * Transfer a byte to the device
   */

  inline uint8_t LTC2986::transferByte(uint8_t rw,uint16_t startAddress,uint8_t inputData) {
    
    uint8_t tx[4],rx[4];

    tx[3]=rw;
    tx[2]=startAddress >> 8;
    tx[1]=startAddress;
    tx[0]=inputData;
    
    spiTransferBlock(tx,rx,4);
    return rx[0];
  }


  /*
   * Write a block and read the response
   */

  inline void LTC2986::spiTransferBlock(const uint8_t *tx,uint8_t *rx,uint8_t length) {
      
    int8_t i;

    GpioSpiCsLtc2986::reset();
    
    for(i=length-1;i>=0;i--) {

      SPDR=tx[i];                     // start transmitting
      while((SPSR & (1<<SPIF))==0);   // wait until transfer ends
      rx[i]=SPDR;                     // get the incoming byte 
    }
    
    GpioSpiCsLtc2986::set();
  }


  /*
   * Run the conversion and get the results
   */


  inline void LTC2986::run(RtdReadings& results) {

    measureChannel(4,TEMPERATURE,results.rtd1);      // Ch 4: RTD PT-100
    MillisecondTimer::delay(25);
    measureChannel(8,TEMPERATURE,results.rtd2);      // Ch 8: RTD PT-100
  }


  /*
   * Measure a channel
   */

  inline void LTC2986::measureChannel(uint8_t channelNumber,uint8_t channelOutput,RtdReadings::Result& result) {
    convertChannel(channelNumber);
    getResult(channelNumber,channelOutput,result);
  }


  inline void LTC2986::convertChannel(uint8_t channelNumber) {
    transferByte(WRITE_TO_RAM, COMMAND_STATUS_REGISTER, CONVERSION_CONTROL_BYTE | channelNumber);
    waitForProcessToFinish();
  }


  inline void LTC2986::waitForProcessToFinish() {
    uint8_t process_finished = 0;
    uint8_t data;
    
    while (process_finished == 0) {
      data = transferByte(READ_FROM_RAM, COMMAND_STATUS_REGISTER, 0);
      process_finished  = data & 0x40;
    }
  }


  inline void LTC2986::getResult(uint8_t channelNumber,uint8_t channelOutput,RtdReadings::Result& result) {

    uint32_t raw_data;
    uint16_t start_address = getStartAddress(CONVERSION_RESULT_MEMORY_BASE, channelNumber);
    uint32_t raw_conversion_result;

    raw_data = transferFourBytes(READ_FROM_RAM,start_address,0);

    // 24 LSB's are conversion result
    
    raw_conversion_result = raw_data & 0xFFFFFF;
    setConversionResult(raw_conversion_result,channelOutput,result);

    // If you're interested in the raw voltage or resistance, use the following
    //if (channelOutput != VOLTAGE)
      //readVoltageOrResistanceResults(channelNumber);

    // 8 MSB's show the fault data
    
    result.faultCode = raw_data >> 24;
  }


  #if 0
  void read_voltage_or_resistance_results(uint8_t channelNumber) {
    
    int32_t raw_data;
    double voltage_or_resistance_result;
    uint16_t start_address = get_start_address(VOUT_CH_BASE, channelNumber);

    raw_data = transfer_four_bytes(READ_FROM_RAM, start_address, 0);
    voltage_or_resistance_result = (double)raw_data/1024;
    Serial.print(F("  Voltage or resistance = "));
    Serial.println(voltage_or_resistance_result);
  }
  #endif


  inline void LTC2986::setConversionResult(uint32_t raw_conversion_result, uint8_t channelOutput,RtdReadings::Result& result) {
    
    int32_t signed_data = raw_conversion_result;
    double scaled_result;

    // Convert the 24 LSB's into a signed 32-bit integer
    
    if(signed_data & 0x800000)
      signed_data = signed_data | 0xFF000000;

    if (channelOutput == TEMPERATURE) {
      scaled_result = static_cast<double>(signed_data) / 1024;
      result.temperature=scaled_result;
    }
    else if (channelOutput == VOLTAGE) {
      scaled_result = static_cast<double>(signed_data) / 2097152;
      result.directAdcReading=scaled_result;
    }
    else if (channelOutput == CODE)
      result.directAdcCode=signed_data;
  }


  inline uint16_t LTC2986::getStartAddress(uint16_t base_address, uint8_t channel_number) {
    return base_address + 4 * (channel_number-1);
  }
}
