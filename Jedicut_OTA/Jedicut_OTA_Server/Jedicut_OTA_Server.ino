/*  Copyright 2018 Javier Hernandez
 *  A heavy modification of  fcifmdlcnc Copyright 2013 Martin
    This file is part of jedicutplugin.
    Jedicut_OTA_Server.ino is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Jedicut_OTA_Server.ino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.*/

/*
The jedicut communication configuration for Clock and Direction must be fixed set to the below values, 
only the wires to the specific board may be different -- wire to -->
Function               Number in jedicut        Arduino Pin            e.g. TB6560 SubD25 pin
                           configuration
                           (fixed !!)
EngineX1 Clock          2                                 D1      -- wire to -->       16   
EngineX2 Clock          3                                 D2     -- wire to -->        9
EngineY1 Clock          4                                 D3      -- wire to -->        14
EngineY2 Clock          5                                 D4     -- wire to -->        3
EngineX1 Direction     6                                 D5      -- wire to -->        1
EngineX2 Direction     7                                 D6     -- wire to -->        8
EngineY1 Direction     8                                 D7     -- wire to -->        7
EngineY2 Direction     9                                 D8      -- wire to -->        6
All Engines On/Off     -                                  D0      -- wire to -->       17; 4 and 5
Heating On/off         -                                   N/A     
Heating PWM            -                                  N/A    
+5V                         -                                  5V       -- wire to -->  SubD 15 Pin 15
Ground                     -                                  G        -- wire to -->  SubD 15 Pin 14
*/ 
/*
Release operative for TB6560 4 axis board and Wemos D1 mini ESP8266 based arduino IDE compatible board
Other motor controller board or esp8266 boards pinout should be changed acordingly
No wire heating control
https://github.com/jedicut/jedicut-devices-extensions */
//This SW is the Server side and should be used at the motor controller end in conjuntion with  Jedicut_OTA_Client at the PC side that runs JEDICUT.exe with USBSerial.dll as comunication plugin.

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
WiFiServer server(4998);
WiFiClient client;

/////////////// You are likely to make changes here:
const char* ssid = "jedicut";             //Write here the name of the on purpose wifi you want create
const char* password = "clino2001"; //Write here the password, must be longer than 8 chars
int pines[] = {D1, D2, D3, D4, D5, D6, D7, D8}; // list of digital outs to manage motors of your board, note D1... is the name of a variable that contains a GPiO number                             
int stepmin_period = 200;              //microseconds, Fast Speed (here is 5.000Hz pulse rate)  change this to change desired speed, the limit: overlapping interrupts... 
int stepmax_period = 1000;           //microseconds,  Cut speed (here is 1.000Hz pulse rate)   change this to change desired speed, the limit: overlapping interrupts... 
int step_period = stepmin_period;  //  period of step pulses in microseconds, either the max or the min
int tickPeriod = 90;    //   motor pulses lenght, in microsecs, change this to suit your board performance
/////////////
#define CMD_BUFFER_SIZE 2048  // must be even !
byte cmdArray[CMD_BUFFER_SIZE];
int arrayIdxRead  = 0;
int arrayIdxWrite = 0;
int cmdCounter = 0;
int wifi_received_bytes = 0;
boolean ovf = false;
bool isrActive = false;
unsigned int val = 0; // The command parameter value
unsigned char BufferRecep[2] = {0};
boolean newcmdreceived = 0;
char mssg[] = {'L'};
long komand_tot_count = 0;
char msg_old = 'L';
int timemark=0;


