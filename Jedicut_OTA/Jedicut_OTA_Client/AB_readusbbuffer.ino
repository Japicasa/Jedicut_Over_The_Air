void readusbbuffer() {
  oldidx = arrayIdxWrite ;
  if (Serial.available() > 1)
  {
    usb_received_bytes = Serial.available();
    if ((usb_received_bytes + cmdCounter) > (CMD_BUFFER_SIZE - 150)) {
      usb_received_bytes = CMD_BUFFER_SIZE - cmdCounter - 150;
      Serial.print("S");
      ovf = true;
    }
    if ((arrayIdxWrite + usb_received_bytes) > CMD_BUFFER_SIZE)
    {
      usb_received_bytes = CMD_BUFFER_SIZE - arrayIdxWrite;
    }
    Serial.readBytes((char*)&cmdArray[arrayIdxWrite], usb_received_bytes);
    // korrekt the write index
    arrayIdxWrite +=  usb_received_bytes;
    cmdCounter    +=  usb_received_bytes;
    if (arrayIdxWrite == CMD_BUFFER_SIZE)
    {
      arrayIdxWrite = 0;
    } 
  }
  int timemark=micros();
  while( (micros()<(timemark+delay2receive))&&(Serial.available()<100))
  {}
  // delayMicroseconds(4500);
} 
