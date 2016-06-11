// Geofence
//
//    Copyright (c) 2016 orangkucing
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Geofence.h"

// Define to use a geofence functionality.
// Teensy 3.x or Arduino Due is recommended because they can do with 64-bit double float.
#define USE_GEOFENCE

// Define for both writing GPS log to and reading geofence setting from microSD.
// SdFat library is downloadable from https://github.com/greiman/SdFat 
#define USE_MICRO_SD
#include <SdFat.h>

// To use Geofence with I2C LCD please download one of libraries from
// http://n.mtng.org/ele/arduino/i2c.html (the webpage is written in Japanese)
// or somewhere else, and uncomment the follwing three lines:
//#define USE_LIQUID_CRYSTAL
//#include <I2CLiquidCrystal.h>
//#include <Wire.h>

// GPS specific settings
// choose your GPS
#define GPS_MEDIATEK

void GPSinit()
{
/* MediaTek MT3339
      TAIYO YUDEN GYSFDMAXB (9600 baud) */
#ifdef GPS_MEDIATEK
#define GPSBAUDRATE 9600
  GPS.print(F("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0*28\r\n")); // enable GPGGA and GPZDA outputs once per second
#endif

/* SiRF
      Position GPS-54 (9600 baud) */
#ifdef GPS_SIRF
#define GPSBAUDRATE 9600
  GPS.print(F("$PSRF106,21*0F\r\n"));                 // set WGS-84 coordinate
#endif

/* SiRFstarIII
      Aitendo GM-318 (4800 baud)
   SiRF-IV
      CanMore GMS6-CR6 (older unit: 4800 baud, newer unit: 9600 baud) */
#ifdef GPS_SIRFIII
//#define GPSBAUDRATE 4800
#define GPSBAUDRATE 9600
  GPS.print(F("$PSRF103,08,00,01,01*2D\r\n"));        // enable GPZDA output once per second
#endif

/* ST Microelectronics
      CanMore GMS6-SR6 (9600 baud) */
#ifdef GPS_STM
#define GPSBAUDRATE 9600
  GPS.print(F("$PSTMSETPAR,1201,0X01000001*71\r\n")); // enable GPGGA and GPZDA outputs only
  GPS.print(F("$PSTMSAVEPAR*58\r\n"));                // save to flash memory
#endif

/* u-blox 6
      Navisys GE-612T (9600 baud)
      Aitendo NEO6M-ANT-4P (9600 baud) */
#ifdef GPS_UBLOX
#define GPSBAUDRATE 9600
  GPS.print(F("$PUBX,40,ZDA,1,1,1,0*45\r\n"));        // enable GPZDA output once per second
  GPS.print(F("$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n"));    // other statement outputs are turned off
  GPS.print(F("$PUBX,40,VTG,0,0,0,0*5E\r\n"));
  GPS.print(F("$PUBX,40,GSV,0,0,0,0*59\r\n"));
  GPS.print(F("$PUBX,40,GSA,0,0,0,0*4E\r\n"));
  GPS.print(F("$PUBX,40,RMC,0,0,0,0*47\r\n"));
#endif
}

void setup() {
  setupGeofence();
  setupSwitch();
  setupGPS();
  setupTrigger();
  setupLiquidCrystal();
}

void loop() {
  checkSwitch();
  checkNewConfig();
  checkGPS();
}
