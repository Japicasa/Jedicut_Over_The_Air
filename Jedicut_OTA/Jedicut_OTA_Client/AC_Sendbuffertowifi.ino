void send_buffer_to_wifi() {
   bytes2send = cmdCounter;
   if (bytes2send>clientpacksize) {
   bytes2send=clientpacksize;
   }
  if (bytes2send > 0) {
    if (TCP_Client.connected())
    {
      if ((arrayIdxRead + bytes2send)>=CMD_BUFFER_SIZE) {
       bytes2send = CMD_BUFFER_SIZE - arrayIdxRead;
      }
      size_t t = bytes2send;
     ///////////////////////////////////////////////////////////////
      
         TCP_Client.write((char*)&cmdArray[arrayIdxRead], t);
      arrayIdxRead = arrayIdxRead + bytes2send;
      if (arrayIdxRead == CMD_BUFFER_SIZE) {
       //oled.println("iBBBB");
        arrayIdxRead = 0;
      }
      cmdCounter -= bytes2send;
    }
serverbuffstatus='T';
Serial.print('S');
while (serverbuffstatus!='L')
{
    while (!TCP_Client.available()); 
    if (TCP_Client.available()) {
      serverbuffstatus = TCP_Client.read();
    }
}   
    ovf = false;
    Serial.print('C');
    if (cmdCounter == 0) {
      digitalWrite(D4, LOW);
    }
    else {
      digitalWrite(D4, HIGH);
    }
  }
}
