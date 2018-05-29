#include <NMEAGPS.h>

//======================================================================
//  Program: NMEAdistance.ino
//
//  Description:  Display distance from a base location.
//
//  Prerequisites:
//     1) NMEA.ino works with your device (correct TX/RX pins and baud rate)
//     2) GPS_FIX_LOCATION has been enabled.
//     3) A sentence that contains lat/long has been enabled (GGA, GLL or RMC).
//     4) Your device sends at least one of those sentences.
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


//!!!!IMPORTANT!!!!
// Open the RH_ASK.cpp file from the RadioHead library and uncomment #define RH_ASK_ARDUINO_USE_TIMER2
// If you don't you will see errors about are shared interrput when compiling.

#include <GPSport.h>

#include <RH_ASK.h> 
#include <SPI.h> // Not actually used but needed to compile 

//http://electronics-diy.com/complete-guide-for-rf-433mhz-transmitter-receiver-module-with-arduino.php
int SensorData; 
char SensorCharMsg[4]; 
RH_ASK driver(2000,5,12,8,false);

//------------------------------------------------------------
// Check that the config files are set up properly

#if !defined( NMEAGPS_PARSE_RMC ) &  \
    !defined( NMEAGPS_PARSE_GGA ) &  \
    !defined( NMEAGPS_PARSE_GLL )
  #error You must uncomment at least one of NMEAGPS_PARSE_RMC, NMEAGPS_PARSE_GGA or NMEAGPS_PARSE_GLL in NMEAGPS_cfg.h!
#endif

#if !defined( GPS_FIX_LOCATION )
  #error You must uncomment GPS_FIX_LOCATION in GPSfix_cfg.h!
#endif

NMEAGPS gps;

// The base location, in degrees * 10,000,000
NeoGPS::Location_t base( 368890453L,-761478978L ); //Virginia Beach

void setup()
{
  Serial.begin(9600);  // Debugging only 
  if (!driver.init()) 
    Serial.println("init failed"); 
  DEBUG_PORT.begin(9600);
  DEBUG_PORT.println( F("NMEAdistance.ino started.") );
  DEBUG_PORT.println( F("Looking for GPS device on " GPS_PORT_NAME) );

  gpsPort.begin(9600);

} // setup

void loop()
{
  while (gps.available( gpsPort )) {
    //read lat,lon from other device
    uint8_t buf[12]; 
    uint8_t buflen = sizeof(buf); 
    if (driver.recv(buf, &buflen)) // Non-blocking 
    { 
      int i; 
      // Message with a good checksum received, dump it. 
      Serial.print("Message: "); 
      Serial.println((char*)buf);
      //todo: parse the lat,lon 
      //todo: then set these values to the base
    } 

    //todo: parse the distance recieved and set it as the base.
    gps_fix fix = gps.read(); // save the latest

    // When we have a location, calculate how far away we are from the base location.
    if (fix.valid.location) {
      float range = fix.location.DistanceMiles( base );

      DEBUG_PORT.print( F("Range: ") );
      DEBUG_PORT.print( range );
      DEBUG_PORT.println( F(" Miles") );
      //todo: convert the distance to a visual for lights
      //todo: need to set a max value for showing all five lights
      //todo: then divide by five to figure out the range to show which lights
      //given a max rf range of 100 meters, at 100 meters all five lights will be on
      //given 20 meters only one light will be on for distance

      //if the following is possible.
      //todo: determine bearing and figure out which lights to show, given n,s,e,w and ne,nw,se,sw, if the other person is north, and the 
      //current person is heading south, then make the bottom lighth light up so they know to turn around.
      //if the handheld is pointing north, and the sending unit is pointing north, then the top light will be showing.
    } else
      // Waiting...
      DEBUG_PORT.print( '.' );
  }
  
} // loop
