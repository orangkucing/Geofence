// generic GPS
//
// copyright 2016 (c) orangkucing
//

void setupGPS()
{
  digitalWrite(LED_YELLOW, LOW);
  pinMode(LED_YELLOW, OUTPUT);

// Start GPS
  GPS.begin(GPSBAUDRATE);

  // following line is unnecessary except Arduino Due
  Serial_begin(GPSBAUDRATE);

  while (GPS.read() <= 0)
    ; // wait GPS to boot 
  GPSinit();
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
