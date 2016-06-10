// generic GPS
//
// copyright 2016 (c) orangkucing
//

void setupGPS()
{
  digitalWrite(LED_YELLOW, LOW);
  pinMode(LED_YELLOW, OUTPUT);
// Start GPS
//#define GPSBAUDRATE 4800
#define GPSBAUDRATE 9600

  GPS.begin(GPSBAUDRATE);

  // following line is unnecessary except Arduino Due
  Serial_begin(GPSBAUDRATE);

  while (GPS.read() <= 0)
    ; // wait GPS to boot
  
// GPS specific settings

/* MediaTek MT3339
      TAIYO YUDEN GYSFDMAXB (9600 baud) */
#if 0
  GPS.print(F("$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0*28\r\n")); // enable GPGGA and GPZDA outputs once per second
#endif

/* SiRF
      Position GPS-54 (9600 baud) */
#if 0
  GPS.print(F("$PSRF106,21*0F\r\n"));                 // set WGS-84 coordinate
#endif

/* SiRFstarIII
      Aitendo GM-318 (4800 baud)
   SiRF-IV
      CanMore GMS6-CR6 (older unit: 4800 baud, newer unit: 9600 baud) */
#if 0
  GPS.print(F("$PSRF103,08,00,01,01*2D\r\n"));        // enable GPZDA output once per second
#endif

/* ST Microelectronics
      CanMore GMS6-SR6 (9600 baud) */
#if 0
  GPS.print(F("$PSTMSETPAR,1201,0X01000001*71\r\n")); // enable GPGGA and GPZDA outputs only
  GPS.print(F("$PSTMSAVEPAR*58\r\n"));                // save to flash memory
#endif

/* u-blox 6
      Navisys GE-612T (9600 baud)
      Aitendo NEO6M-ANT-4P (9600 baud) */
#if 1
  GPS.print(F("$PUBX,40,ZDA,1,1,1,0*45\r\n"));        // enable GPZDA output once per second
  GPS.print(F("$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n"));    // other statement outputs are turned off
  GPS.print(F("$PUBX,40,VTG,0,0,0,0*5E\r\n"));
  GPS.print(F("$PUBX,40,GSV,0,0,0,0*59\r\n"));
  GPS.print(F("$PUBX,40,GSA,0,0,0,0*4E\r\n"));
  GPS.print(F("$PUBX,40,RMC,0,0,0,0*47\r\n"));
#endif

}

void __checkGPS()
{
  Serial_print(gpsbuf);

  if (strncmp_P(gpsbuf, PSTR("$GPGGA"), 6) == 0) { // GPGGA or GPGGA5
    int s = -1, cc;
    for (int i = 0; i < 6; i++) {
      s = ((String)gpsbuf).indexOf(',', s + 1);
    }
    cc = ((String)gpsbuf).charAt(s + 1);
    // 0: Invalid GPS fix, 6: Estimated fix, 7: Manual input, 8: Simulation
    // 1: SPS, 2: DGPS, 3: PPS, 4: RTK, 5: Float RTK
    if ((cc >= '1' && cc <= '5')) {
      digitalWrite(LED_YELLOW, HIGH); // indicate GPS fix
#ifdef USE_GEOFENCE
      //
      {
        boolean geoflag = false;
        double x, y, z;
        s = -1;
        s = ((String)gpsbuf).indexOf(',', s + 1);
        s = ((String)gpsbuf).indexOf(',', s + 1);
        if (_scanCoordinate(gpsbuf + s + 1, &x, &y, &z)) {
          for (int j = 0; j <= polycount; j++) {
            for (int i = 1; i < wpcount[j]; i++) {
              // we use outer product and inner product
              // \langle \mathbf x_{i-1} \otimes \mathbf x_i, \mathbf x \rangle > 0
              if ((geofence[j][i - 1][GEO_Y] * geofence[j][i][GEO_Z] * x + geofence[j][i - 1][GEO_Z] * geofence[j][i][GEO_X] * y + geofence[j][i - 1][GEO_X] * geofence[j][i][GEO_Y] * z) >
                  (geofence[j][i - 1][GEO_Z] * geofence[j][i][GEO_Y] * x + geofence[j][i - 1][GEO_X] * geofence[j][i][GEO_Z] * y + geofence[j][i - 1][GEO_Y] * geofence[j][i][GEO_X] * z)) {
                geoflag = true;
              } else {
                geoflag = false;
                break;
              }
            }
            if (geoflag) {
              break;
            }
          }
        } else {
          geoflag = false;
        }
   
        // geofence debounce
        {
          static unsigned long lastDebounceTime = 0;
          // default to outside
          static boolean lastButtonState = false;
          static boolean buttonState = false;

          boolean reading = geoflag;
          if (reading != lastButtonState) {
            lastDebounceTime = millis();
          }
          if (millis() - lastDebounceTime >= geofenceSensitivity) {
            if (reading != buttonState) {
              if (reading > 0) {
                sendStart();
              } else {
                sendStop();
              }
              buttonState = reading;
            }
          }
          lastButtonState = reading;
        }
        // end geofence debounce
      }
#endif       
    } else {
      digitalWrite(LED_YELLOW, LOW);
    }
    outputLogFile(gpsbuf);
  } else if (strncmp_P(gpsbuf, PSTR("$GPZDA,"), 7) == 0) {
    // datetime info received
    LCDprint(gpsbuf + 7);
#ifdef USE_MICRO_SD
    {
      int s = 6;
      dGPStype dTmp;
      sscanf_P(gpsbuf + s + 1, PSTR("%2hhu%2hhu%2hhu"), &dTmp.g_hour, &dTmp.g_minute, &dTmp.g_second);
      s = ((String)gpsbuf).indexOf(',', s + 1);
      sscanf_P(gpsbuf + s + 1, PSTR("%2hhu,%2hhu,%4hu"), &dTmp.g_day, &dTmp.g_month, &dTmp.g_year);
      noInterrupts();
      dGPS = dTmp;
      interrupts();
      outputLogFile(gpsbuf);
    }
#endif
  }
}

void checkGPS()
{
  static int ind = 0;
  while (GPS.available()) {
    if (ind >= MAXNMEALEN) {
      // buffer overrun error
      ind = 0;
      continue;
    }
    if ((gpsbuf[ind++] = GPS.read()) == '\n') {
      gpsbuf[ind] = '\0';
      // sanity check
      if (ind > 5 && gpsbuf[0] == '$' && gpsbuf[ind - 5] == '*') {
        unsigned char s = 0;
        unsigned int t;
        sscanf_P(gpsbuf + ind - 4, PSTR("%02X"), &t);
        for (int i = 1; i < ind - 5; i++) {
          s ^= gpsbuf[i];
        }
        if ((unsigned int)s == t) {
          // check sum OK
          __checkGPS();
        }
      }
      ind = 0;
    }
  }  
}
