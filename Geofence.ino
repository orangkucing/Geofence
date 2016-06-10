// Geofence
//
//    Copyright (c) 2016 orangkucing
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Geofence.h"

#define USE_GEOFENCE

#define USE_MICRO_SD
#include <SdFat.h>

//#define USE_LIQUID_CRYSTAL
//#include <I2CLiquidCrystal.h>
//#include <Wire.h>

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
