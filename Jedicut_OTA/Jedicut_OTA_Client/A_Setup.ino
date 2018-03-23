/////////////////////////////////////////
void setup() {
  pinMode(D4, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) ) { // the led will blink until connection to server is successful
    delay(500);
    digitalWrite(D4, !digitalRead(D4));
  }
  Serial.begin(115200);   //must match the baud rate of comport.ini file in the folder where jedicut.exe is placed
  TCP_Client.connect(TCP_Server, TCP_ServerPort);
  TCP_Client.setNoDelay(1);
  if (send_speed); {
    send_speed_settings(); 
  }
}
