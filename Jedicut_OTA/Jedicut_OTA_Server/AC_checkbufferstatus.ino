///////////////////////////////////////////////////
void checkbufferstatus()
{
  if (cmdCounter >= (CMD_BUFFER_SIZE - 600)) //prepare the message for the client to pause sending commands
  {
    ovf = true;
    mssg[0] = 'T';
  }
  else
  {
    if (cmdCounter < CMD_BUFFER_SIZE - 800) //prepare the message for the client to continue sending commands
    {
      //buffer not full: continue sending commands
      mssg[0] = 'L';
    }
  }
  if ((mssg[0] != msg_old) || newcmdreceived) {
    client.write((char*)&mssg[0], 1); //I found usig this format the loop is faster
    msg_old = mssg[0];
    newcmdreceived = 0;
}
}
