/*
 * Andy's Workshop Brewery Relays controller ATMega328p firmware
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

#include "ValidMask.h"
#include "RelayState.h"
#include "Eeprom.h"
#include "GpioPin.h"
#include "ProgStrings.h"
#include "MillisecondTimer.h"
#include "Uart.h"
#include "Relay.h"
#include "TriacControl.h"
#include "Aux2.h"
#include "Id.h"
#include "Capabilities.h"
#include "Copyright.h"
#include "Version.h"
#include "Serial.h"
#include "Uptime.h"
#include "Valid.h"
#include "Chillout.h"
#include "Program.h"
