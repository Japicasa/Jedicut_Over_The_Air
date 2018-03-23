//////////////////////////////////////////////
void readwifi2buffer()
{
  wifi_received_bytes = client.available();
  if (wifi_received_bytes > 0) {
    if ( arrayIdxWrite + wifi_received_bytes > CMD_BUFFER_SIZE) {
      wifi_received_bytes = CMD_BUFFER_SIZE - arrayIdxWrite;
    }
     client.readBytes((char*)&cmdArray[arrayIdxWrite], wifi_received_bytes);
    //Serial.print("wifi_received_bytes");
   // Serial.println(wifi_received_bytes);
    arrayIdxWrite += wifi_received_bytes;
    if (arrayIdxWrite == CMD_BUFFER_SIZE) {
      arrayIdxWrite = 0;
    }
    noInterrupts();
    cmdCounter = cmdCounter + wifi_received_bytes ;
    interrupts();
    newcmdreceived = 1;
  }
}

