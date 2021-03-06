/*  Copyright 2018 Javier Hernandez
 *  A heavy modification of  fcifmdlcnc Copyright 2013 Martin
    This file is part of jedicutplugin.
    Jedicut_OTA_Client.ino is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Jedicut_OTA_Client.ino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
   
*/ 
/* Release  operative for TB6560 4 axis  board and Wemos D1 mini ESP8266 based arduino IDE compatible board
 For other motor controller board or esp8266 board wiring and setting should be changed acordingly
 No wire heating control*/
 //This SW is the Client side and should be used at the PC side that runs JEDICUT.exe with USBSerial.dll as comunication plugin while Jedicut_OTA_Client runs at the motor controller end.
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const char* ssid="jedicut"; //must match server settings
const char* password="clino2001";
int TCP_ServerPort  = 4998;          //Change port# acordingly
IPAddress        TCP_Server(192,168,4,1); //Change IP# acordingly, this is the default IP when Esp8266 is the AP
WiFiClient           TCP_Client;

#define CMD_BUFFER_SIZE 2512   // must be even !
volatile byte cmdArray[CMD_BUFFER_SIZE];
volatile int arrayIdxRead  = 0;
volatile int arrayIdxWrite = 0;
volatile boolean ovf = false;
volatile int cmdCounter = 0;
unsigned int val = 0; // The command parameter value
char serverbuffstatus='L';
int bytes2send=0;
int usb_received_bytes=0;
int oldidx=0;
///////////////////////////////////////////// You are likely to change parameters below to optimise your machine performance
int clientpacksize=250;                                  // max number of bytes to be sent in each TCP socket call
int delay2receive=14500;                              // microseconds time lapse to receive enough bytes from USB
boolean send_speed= true;                          // make send_speed 'true' if you want the below values sent to change Senver´s max and min speed upon Client conection
unsigned int stepmin_period = 220;              //microseconds, Fast Speed (here is 5.000Hz pulse rate)  change this to change desired speed, the low limit: overlapping interrupts... 
unsigned int stepmax_period = 700;           //microseconds,  Cut speed (here is 1.000Hz pulse rate)   change this to change desired speed, the high limit: too low speeds. 
