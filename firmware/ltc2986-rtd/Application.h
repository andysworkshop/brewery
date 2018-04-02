/*
 * Andy's Workshop Brewery LTC2986 RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once

/*
 * This is the one and only header file included by the few .cpp files.
 * There is no need to include any other header anywhere in this project.
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/crc16.h>
 
#include "FirmwareVersion.h"
#include "GpioPin.h"
#include "ProgStrings.h"
#include "MillisecondTimer.h"
#include "AlarmState.h"
#include "Eeprom.h"
#include "Uart.h"
#include "Max7221.h"
#include "LTC2986Constants.h"
#include "RtdReadings.h"
#include "LTC2986.h"
#include "Calibration.h"
#include "AlarmFlasher.h"
#include "Alarm.h"
#include "Id.h"
#include "Copyright.h"
#include "Version.h"
#include "Serial.h"
#include "Capabilities.h"
#include "Uptime.h"
#include "Readings.h"
#include "Displays.h"
#include "CommandProcessor.h"
#include "Program.h"
