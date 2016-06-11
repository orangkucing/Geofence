#define MAXNMEALEN 83  // maximum chars in a line of NMEA sentence

char gpsbuf[MAXNMEALEN];

#ifdef USE_GEOFENCE

// currently waypoint (geofence) sentence is only recognized
#define GEO_X 0
#define GEO_Y 1
#define GEO_Z 2
int wpcount[MAXPOLYGONS];
int polycount;
double geofence[MAXPOLYGONS][MAXWAYPOINTS][3];
unsigned int geofenceSensitivity = 5000; // 5 seconds = 5000 milliseconds

double atod(char *b)
{
  double scale = 0.1;
  double value = 0;
  while (*b >= '0' && *b <= '9') {
    value += (*b++ - '0') * scale;
    scale *= 0.1;
  }
  return value;
}

int _scanCoordinate(char *b, double *x, double *y, double *z)
{
  double latitude, longitude;
  int i = -1, j, tmp;
  tmp = atoi(b + i + 1); // integral part
  latitude = tmp % 100;
  tmp /= 100;
  j = ((String)b).indexOf('.', i + 1);
  i = ((String)b).indexOf(',', i + 1);
  if (j >= i) {
      return 0;
  }
  if (j != -1) {
    latitude += atod(b + j + 1); // fractional part
  }
  latitude = latitude / 60.0 + tmp; // degree
  latitude *= PI / 180.0; // radian
  if (b[i + 1] == 'S') {
    latitude *= -1.0;
  }
  if ((i = ((String)b).indexOf(',', i + 1)) == -1) {
    return 0;
  }
  tmp = atoi(b + i + 1); // integral part
  longitude = tmp % 100;
  tmp /= 100;
  j = ((String)b).indexOf('.', i + 1);
  i = ((String)b).indexOf(',', i + 1);
  if (j >= i) {
    return 0;
  }
  if (j != -1) {
    longitude += atod(b + j + 1); // fractional part
  }
  longitude = longitude / 60.0 + tmp; // degree
  longitude *= PI / 180.0; // radian
  if (b[i + 1] == 'W') {
    longitude *= -1.0;
  }
  if ((i = ((String)b).indexOf(',', i + 1)) == -1) {
    return 0;
  }
  *z = sin(latitude);
  *x = cos(latitude) * cos(longitude);
  *y = cos(latitude) * sin(longitude);
  return 1;
}

void parseConfig(char *b)
{
  double x, y, z;
  
  if (strncmp_P(b, PSTR("$GPWPL,"), 7) == 0) {
    if (strncmp_P(b + 7, PSTR(",,,"), 3) == 0) {
      // new polygon
      polycount++;
      wpcount[polycount] = 0;
    } else {
      if (_scanCoordinate(b + 7, &x, &y, &z)) {
        geofence[polycount][wpcount[polycount]][GEO_X] = x;
        geofence[polycount][wpcount[polycount]][GEO_Y] = y;
        geofence[polycount][wpcount[polycount]][GEO_Z] = z;
        wpcount[polycount]++;
      }
    }
  } else if (strncmp_P(b, PSTR("$PMEWC,"), 7) == 0) {
    int d;
    char c;
    sscanf_P(b + 7, PSTR("%d,%c"), &d, &c);
    switch (c) {
      case 'S': // sensitivity of geofence
        geofenceSensitivity = d;
        break;
      default:
        // unknown setting request
        break;
    }
  } else {
    // unsupported sentence
  }
}

void parseConfig(const __FlashStringHelper *b)
{
  int i = 0;
  while ((*(gpsbuf + i) = pgm_read_byte_near((const PROGMEM char *)b + i)) != '\0') {
    i++;
  }
  parseConfig(gpsbuf);
}

void setupGeofence()
{
  // constant geofence burnt in source code
  polycount = 0;
  wpcount[polycount] = 0;
  // geofence here! be careful not to exceed the limits: MAXWAYPOINTS, MAXPOLYGONS
  //parseConfig(F("$GPWPL,"));
}

#else

void setupGeofence()
{
  
}

#endif

#ifdef USE_MICRO_SD

#define CONFIGFILE "AUTOEXEC.TXT"
#define LOGFILE "logfile.txt"

// the following time in UTC will be updated by GPS
typedef struct dGPStype {
  uint16_t g_year;
  uint8_t g_month;
  uint8_t g_day;
  uint8_t g_hour;
  uint8_t g_minute;
  uint8_t g_second;
} dGPStype;

dGPStype dGPS = {
  // dummy date 1 Jan 2000;
  2000, 1, 1,
  // dummy time 00:00:00
  0, 0, 0
};


void outputLogFile(const char *buf)
{
  SdFile dataFile;
  if (microSDState) {
    dataFile.open(LOGFILE, O_WRITE | O_CREAT | O_APPEND);
    if (dataFile.isOpen()) {
      dataFile.print(buf);
      dataFile.close();
    }
  }
}

// callback for SdFat lib
void SDtimeStamp(uint16_t *d, uint16_t *t)
{
  *d = FAT_DATE(dGPS.g_year, dGPS.g_month, dGPS.g_day);
  *t = FAT_TIME(dGPS.g_hour, dGPS.g_minute, dGPS.g_second);
}

void checkNewConfig()
{
#ifdef USE_GEOFENCE
  char line[MAXNMEALEN + 1];
  SdFile configFile;
  
  if (microSDState != 1) {
    return;
  }
  microSDState = 2;

  // read configurations from CONFIGFILE
  configFile.open(CONFIGFILE, O_READ);
  if (configFile.isOpen()) {
    polycount = 0;
    wpcount[polycount] = 0;
    while (configFile.fgets(line, sizeof(line)) && wpcount[polycount] < MAXWAYPOINTS - 1 && polycount < MAXPOLYGONS) {
      parseConfig(line);
    }
    configFile.close();
  }
#endif
}

#else

void outputLogFile(const char *s)
{
  
}

void checkNewConfig()
{
 
}

#endif

