# Geofence App
General purpose DIY geofence module using Teensy 3.x and GPS.

## Overview
This DIY unit continuously receives the signals from satellites and raises alarms when it enters/exits predesignated virtual geographical area called _Geofence_. 

This is an open hardware/software device using the following:

* Teensy 3.x and Arduino IDE with SdFat library.
* MicroSD card slot.
* GPS receiver with TTL UART.

## Hardware

[Schematics](http://mewpro.cc/wp-content/uploads/Geofence.pdf)

>We use [TAIYOYUDEN GYSFDMAXB (webpage in Japanese)](http://akizukidenshi.com/catalog/g/gK-09991/) as the GPS receiver but you can use almost any one of 3V3-level UART in the schematic.

SHUTTER pin (Teensy 3.x D2 pin) becomes LOW when the unit enters the geofence, hi-Z or floating when exits.  You can connect the pin and GND to [MewPro](http://mewpro.cc) or something you wish to control. 
 
## Configuration
The geofence feature is controlled by a file on microSD card inserted to the slot.

>The microSD logs all the NMEA sentences from the GPS receiver as well as geofence events, enter or exit, in a file named `logfile.txt`.

The configuration must be stored in `AUTOEXEC.TXT` file and if it is found at the root of microSD media then the application reads it and changes the fenced area accordingly.  Since the slot is PnP hardware, the file is searched every time when the card is inserted or the module boots.

The following is an example of AUTOEXEC.TXT:

~~~
$GPWPL,3541.02806,N,13945.09522,E,Tokyo (waypoint 1)
$GPWPL,4303.75798,N,14121.20388,E,Sapporo (waypoint 2)
$GPWPL,3955.48302,N,11623.80506,E,Beijing (waypoint 3)
$GPWPL,2502.0523,N,12131.24062,E,Taipei (waypint 4)
$GPWPL,3541.02806,N,13945.09522,E,Tokyo (close path)
$GPWPL,,,,,first polygon end; second polygon begin
$GPWPL,3542.38568,N,13940.35048,E,OrangKucing Lab
$GPWPL,3542.60732,N,13939.98916,E,Ramen shop
$GPWPL,3542.3576,N,13939.92706,E,Nakano Station
$GPWPL,3542.38568,N,13940.35048,E,OrangKucing Lab (close path)
~~~

The first five lines, whose first and last are identical, define a geometrical boundary of a counter-clockwisely-closed convex 4-point-polygon.  It may be easy to understand that each of the lines are the waypoint sentence of standard NMEA: Waypoint names or the last fields of lines are ignored when defining geofences. 

>Note: Care should be taken a line should not exceed 82 characters including the trailing \r\n (MSDOS style end of line: carriage return + line feed).  

The defined fence is a large area of the earth (Tokyo - Sapporo - Beijing - Taipei), and if the unit enters to the area it will send an alarm.  And then exiting from there it will raise another alarm, too.

A geofence is not necessarily a closed polygon, that is if the first waypoint and the last differ then they will define a half line or ray.

Moreover if the waypoint is ordered as clockwise, i.e., in the above example but changed like Tokyo - Taipei - Beijing - Sapporo - Tokyo, then the sequence defines another geofence for **outside** of the polygon.  This means if the unit enters to the polygon then it emits the "exit" alarm, whereas exits from there the "enter" alarm.

Current application can handle maximum of 64 geofences each of which is a convex polygon. These geofences are separated by an empty waypoint sentense as seen in the above example.  One geofence must not exceed 32 points.  These limitations are due to memory amout of Teensy 3.x.

## Google Maps URL to GPWPL - NMEA waypoints

For easy making of geofence configuration file, we prepare a dedicated webpage [Google Maps URL to GPWPL - NMEA waypoints](http://www.mewpro.cc/GPWPL.html).

You can just copy URL in the address bar after you set pins to Google Maps, and paste it to the input box in the above webpage.

## Note on USB Battery Auto Shutdown

The unit draws very small amount of current (10mA - 20mA) from the USB battery attached.  It sometimes makes trouble for us that USB battery is automatically shutdown after every power on in 10 seconds or so.

A workaround is to attach a dummy load between USB VBUS (+5V) and USB GND. Please also refer this post [USB Battery Bank Prevent Shut Down](https://forum.pjrc.com/threads/28624-USB-Battery-Bank-Prevent-Shut-Down) for more elegant solutions.

Or using LiPo battery cells and a voltage boost converter to supply 5V solves the issue.
