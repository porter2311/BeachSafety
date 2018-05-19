#include <NMEAGPS.h>

//======================================================================
//  Program: NMEAsimple.ino
//
//  Description:  This program shows simple usage of NeoGPS
//
//  Prerequisites:
//     1) NMEA.ino works with your device (correct TX/RX pins and baud rate)
//     2) At least one of the RMC, GGA or GLL sentences have been enabled in NMEAGPS_cfg.h.
//     3) Your device at least one of those sentences (use NMEAorder.ino to confirm).
//     4) LAST_SENTENCE_IN_INTERVAL has been set to one of those sentences in NMEAGPS_cfg.h (use NMEAorder.ino).
//     5) LOCATION and ALTITUDE have been enabled in GPSfix_cfg.h
//
//  'Serial' is for debug output to the Serial Monitor window.
//
//  License:
//    Copyright (C) 2014-2017, SlashDevin
//
//    This file is part of NeoGPS
//
//    NeoGPS is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NeoGPS is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with NeoGPS.  If not, see <http://www.gnu.org/licenses/>.
//
//======================================================================


!!!!IMPORTANT!!!!
// Open the RH_ASK.cpp file from the RadioHead library and uncomment #define RH_ASK_ARDUINO_USE_TIMER2
// If you don't you will see errors about are shared interrput when compiling.

#include <GPSport.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver(2000,4,12,8,false);;

NMEAGPS  gps; // This parses the GPS characters
gps_fix  fix; // This holds on to the latest values

#define rfTransmitPin 4  //RF Transmitter pin = digital pin 4

void setup()
{
  pinMode(rfTransmitPin, OUTPUT);
  if (!driver.init())
         Serial.println("rf failed");
         
  DEBUG_PORT.begin(9600);
  while (!Serial)
    ;
  DEBUG_PORT.print( F("NMEAsimple.INO: started\n") );

  gpsPort.begin(9600);
}

//--------------------------

void loop()
{
  while (gps.available( gpsPort )) {
    fix = gps.read();

    DEBUG_PORT.print( F("Location: ") );
    if (fix.valid.location) {
      DEBUG_PORT.print( fix.latitude(), 6 );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( fix.longitude(), 6 );
      
      //build lat and long string separated by a comma
      // probably better to just put in a char array, but this was quick, and not sure if I need 20 chars, I just picked something  
      String buf;
      buf += String(fix.latitude(), 6);
      buf += F(",");
      buf += String(fix.longitude(), 6);
     
      //prepare and send string
      char msg[20];
      buf.toCharArray(msg,20);
      driver.send((uint8_t *)msg, 20);
    }

    DEBUG_PORT.println();
  }
}
