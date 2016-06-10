#if defined(__MK20DX256__) || defined(__MK20DX128__) || defined(__MKL26Z64__)
// Teensy 3.x/LC pins
//
//                           0;   // UART0_RX (Serial1 RxI port)
//                           1;   // UART0_TX (Serial1 TxO port)
const int SHUTTER          = 2;   // output LOW when shooting video
//                           3;   // (Not in use)
//                           4;   // (Not in use)
//                           5;   // (Not in use)
//                           6;   // (Not in use)
//                           7;   // (Not in use)
//                           8;   // (Not in use)
const int LED_YELLOW       = 9;   // LED ON: HIGH
const int SPI_CS           = 10;  // (SS)
const int SPI_MOSI         = 11;  // default MOSI
const int SPI_MISO         = 12;  // default MISO
const int SPI_SCK          = 13;  // default SCK / built-in LED
//                           14;  // (A0) alternate SCK
const int CARD_DETECT      = 15;  // (A1) microSD card inserted: LOW
//                           A2;  // (16) (Not in use)
//                           A3;  // (17) (Not in use)
//                           A4;  // (18) (I2C SDA0)
//                           A5;  // (19) (I2C SCL0)
//                           A6;  // (20) (Not in use)
//                           A7;  // (21) (Not in use)
//                           A8;  // (22) (Not in use)
//                           A9;  // (23) (Not in use)
//                                    24; // (Not in use)
//                                    25; // (Not in use)
// Teensy 3.x only
//                           A10; // Analog only (Not in use)
//                           A11; // Analog only (Not in use)
//                           A12; // Analog only (Not in use)
//                           A13; // Analog only (Not in use)
//                           A14; // DAC (Not in use)
//                           A15; // (26) (Not in use)
//                           A16; // (27) (Not in use)
//                           A17; // (28) (Not in use)
//                           A18; // (29) (Not in use)
//                           A19; // (30) (Not in use)
//                           A20; // (31) (Not in use)
//                                    32; // (Not in use)
//                                    33; // (Not in use)
#define sscanf_P sscanf
#define MONITOR Serial  // USB
#define GPS Serial1     // connect to GPS

#elif defined(__AVR_ATmega32U4__)
// Arduino Pro Micro (jumper set to 3.3V) / Arduino Leonardo (3.3V mod) pins
//
//                           0;   // UART0_RX (Serial1 RxI port)
//                           1;   // UART0_TX (Serial1 TxO port)
//                           2;   // (Used by I2C SDA)
//                           3;   // (Used by I2C SCL)
//                           4;   // (24 | A6)
const int SHUTTER          = 5;   // output LOW when shooting video
//                           6;   // (25 | A7) (Not in use)
//                           7;   // (Not in use)
//                           8;   // (26 | A8) (Not in use)
//                           9;   // (27 | A9) (Not in use)
const int SPI_CS           = 10;  // (28 | A10)
//                           11;  //               (Arduino Pro Micro: No pin)
//                           12;  // (29 | A11)    (Arduino Pro Micro: No pin)
const int LED_YELLOW       = 13;  // built-in LED  (Arduino Pro Micro: No pin, No LED)
const int SPI_MOSI         = MOSI;// (16: at ICSP header)
const int SPI_MISO         = MISO;// (14: at ICSP header)
const int SPI_SCK          = SCK; // (15: at ICSP header)
//                           SS;  // (17) RXLED No pin
//                                // (30) TXLED No pin
const int CARD_DETECT      = A0;  // (18) microSD card inserted: LOW
//                           A1;  // (19) (Not in use)
//                           A2;  // (20) (Not in use)
//                           A3;  // (21) (Not in use)
//                           A4;  // (22)          (Arduino Pro Micro: No pin)
//                           A5;  // (23)          (Arduino Pro Micro: No pin)

#define MONITOR Serial  // USB
// Note. We can't use buffer-less MinimumSerial.h here because GPS sends a lot of data!
#define GPS Serial1

#elif defined(__SAM3X8E__)
// Arduino Due pins
//
//                           0;   // (Serial RX0 port: connected to 16U2 used for USB programming port)
//                           1;   // (Serial TX0 port: connected to 16U2 used for USB programming port)
//                           2;   // (Not in use)
//                           3;   // (Not in use)
//                           4;   // (87) (alternate SS)
const int SHUTTER          = 5;   // output LOW when shooting video
//                           6;   // (Not in use)
//                           7;   // (Not in use)
//                           8;   // (Not in use)
//                           9;   // (Not in use)
const int SPI_CS           = 10;  // (77) (SS)
//                           11;  // (Not in use)
//                           12;  // (Not in use)
const int LED_YELLOW       = 13;  // built-in LED
//
//                                // (70) Wire1 I2C SDA1
//                                // (71) Wire1 I2C SCL1
//
const int SPI_MOSI         = MOSI;// at SPI header
const int SPI_MISO         = MISO;// at SPI header
const int SPI_SCK          = SCK; // at SPI header
//
//                           14;  // (Serial3 TX3 port: Not in use)
//                           15;  // (Serial3 RX3 port: Not in use)
//                           16;  // (Serial2 TX2 port: Not in use)
//                           17;  // (Serial2 RX2 port: Not in use)
//                           18;  // Serial1 TX1 port
//                           19;  // Serial1 RX1 port
//                           20;  // (Wire I2C SDA: Not in use)
//                           21;  // (Wire I2C SCL: Not in use)
//
//                                // (22--53) (Not in use)
//
const int CARD_DETECT      = A0;  // (54) microSD card inserted: LOW
//                           A1;  // (55) (Not in use)
//                           A2;  // (56) (Not in use)
//                           A3;  // (57) (Not in use)
//                           A4;  // (58) (Not in use)
//                           A5;  // (59) (Not in use)
//                           A6;  // (60) (Not in use)
//                           A7;  // (61) (Not in use)
//
//                           A8;  // (62) (Not in use)
//                           A9;  // (63) (Not in use)
//                           A10; // (64) (Not in use)
//                           A11; // (65) (Not in use)
//                           66;  // (DAC0: Not in use)
//                           67;  // (DAC1: Not in use)
//                           68;  // (CANRX: Not in use)
//                           69;  // (CANTX: Not in use)
//
//                           72;  // RXLED
//                           73;  // TXLED

#define sscanf_P sscanf
#define MONITOR Serial
#define GPS Serial1     // connect to GPS

#elif defined(__AVR_ATmega328P__)
// Arduino Uno (3.3V mod) / Arduino Pro Mini 3.3V
//
//                           0;   // UART0_RX (Serial RxI port)
//                           1;   // UART0_TX (Serial TxO port)
const int SOFTSERIAL_RX    = 2;   // Connected to GPS
const int SOFTSERIAL_TX    = 3;   // Connected to GPS
//                           4;   // (Not in use)
const int SHUTTER          = 5;   // output LOW when shooting video
//                           6;   // (Not in use)
//                           7;   // (Not in use)
//                           8;   // (Not in use)
const int LED_YELLOW       = 9;   // LED ON: HIGH
const int SPI_CS           = 10;  // SS
const int SPI_MOSI         = 11;  // MOSI
const int SPI_MISO         = 12;  // MISO
const int SPI_SCK          = 13;  // SCK / (built-in LED unusable)
const int CARD_DETECT      = A0;  // (14) microSD card inserted: LOW
//                           A1;  // (15) (Not in use)
//                           A2;  // (16) (Not in use)
//                           A3;  // (17) (Not in use)
//                           A4;  // (18) (I2C SDA0)
//                           A5;  // (19) (I2C SCL0)
//                           A6;  // (20) Analog only (Not in use)
//                           A7;  // (21) Analog only (Not in use)

// It is safe to use buffer-less MinimumSerial.h included in SdFat distribution
// since the serial line is almost output only.
#include <MinimumSerial.h>
#define MONITOR Monitor
MinimumSerial Monitor;
// or
// #define MONITOR Serial
#undef GPS  // undef to use SoftwareSerial library
#include <SoftwareSerial.h>
SoftwareSerial GPS(SOFTSERIAL_RX, SOFTSERIAL_TX);

#endif


// Adjust geofence array size to memory
#if defined(__MK20DX256__)
//     Teensy 3.1/3.2 
#define MAXWAYPOINTS 32
#define MAXPOLYGONS 64

#elif defined(__MK20DX128__)
//     Teensy 3.0
#define MAXWAYPOINTS 16
#define MAXPOLYGONS 16

#elif defined(__MKL26Z64__)
//    Teensy LC
#define MAXWAYPOINTS 8
#define MAXPOLYGONS 1

//    Arduino Pro Micro 3.3V 8MHz / Arduino Leonardo (3.3V mod)
#elif defined(__AVR_ATmega32U4__)
#define MAXWAYPOINTS 8
#define MAXPOLYGONS 8

//    Arduino Due
#elif defined(__SAM3X8E__)
#define MAXWAYPOINTS 16
#define MAXPOLYGONS 16

// Arduino Uno (3.3V mod) / Arduino Pro Mini 3.3V
#elif defined(__AVR_ATmega328P__)
#define MAXWAYPOINTS 8
#define MAXPOLYGONS 4

#else
#error CPU not supported
#endif

#ifdef MONITOR
// use UART
#define Serial_begin MONITOR.begin
#define Serial_read MONITOR.read
#define Serial_print MONITOR.print
#define Serial_println MONITOR.println

#else
// UART related functions must be replaced by dummy functions.
void Serial_begin(...) { }
int Serial_read(...) { return -1; }
void Serial_print(...) { }
void Serial_println(...) { }

#endif
