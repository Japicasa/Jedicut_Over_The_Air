/////////////////////////////////////////////////////
void loop() {
  if (!client.connected())
  {
    client = server.available();
  }
  else
  {
    readwifi2buffer();
    checkbufferstatus();
   
    if (timemark>0){ ///Pause management
      isrActive = true; //the interrupt function is inhibited until "timemark" is reached
      if(millis()>timemark){
        isrActive = false;
        timemark=0;
      }
    }
   }
}


