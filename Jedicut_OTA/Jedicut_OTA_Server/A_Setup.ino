/////////////////////////////////////////
void setup() {
  Serial.begin(115200); //comment this if no Serial Comms are needed
  pinMode(D0, OUTPUT); // All axis on/off
  pinMode(D5, OUTPUT); // Motor Pins  Direction
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D1, OUTPUT); // Motor Pins Step/Clock
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D0, LOW); //disable motors board

  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP);

  server.begin();
  server.setNoDelay(1);
  Serial.println("Server started");
  Serial.setDebugOutput(true);
  /////////Below the interrupt setup for ESP8266 Thanks, OTP see: http://onetechpulse.com/viewtopic.php?t=2
  timer1_isr_init();
  timer1_attachInterrupt(flip);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);  
  timer1_write((clockCyclesPerMicrosecond() / 16) * step_period);
}

