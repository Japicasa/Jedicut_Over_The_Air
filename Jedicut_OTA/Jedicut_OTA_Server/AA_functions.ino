///////////////////////////////////
void  sendMotorCmd(byte cmd) {
  // "pines[]" 4-7 direction // "pines[]" 0-3 step  // we set direction first
  for (int i = 7; i >= 0; i--) {
    digitalWrite(pines[i], !bitRead(cmd, i)); //seems TB6560is falling edge triggering, we change the actual Jedicut command
    //Direction of motors should be "inversed"  as needed in Jedicut Options/CNC controller screen
  }
  delayMicroseconds(tickPeriod); // and then trip back the step pins after some time (tickperiod)
  for (int i = 3; i >= 0; i--) {
    digitalWrite(pines[i], 1);
  }
}

////////////////////////////////////////
void handleCommand() {
  val = cmdArray[arrayIdxRead + 1]; // The command parameter value
  switch (cmdArray[arrayIdxRead])
  {
    case 'A':   // All Motors on/off
      {
        if (val == '1')  {
          //digitalWrite(D4, LOW);
          digitalWrite(D0, HIGH); // Enable motors
        }
        else  {
          digitalWrite(D0, LOW); // Disable motors
          Serial.print("No. of commands ");  Serial.print(komand_tot_count); Serial.print(" Last Comand was: "); Serial.print(char(cmdArray[arrayIdxRead]));
          Serial.print(" arrayIdxRead was: ");  Serial.print(arrayIdxRead); Serial.print(" arrayIdxWrite was: "); Serial.print(arrayIdxWrite);
          Serial.print(" cmdcounter was: ");  Serial.print(cmdCounter); Serial.println(" End_of_Cut_session");
          komand_tot_count = 0;
        }
        break;
      }
    case 'H':   // Wire Heat ON/OFF (may be programmed as PWM (analog out))
      {
        //Heating is not managed in this version since Wemos D1 mini has only nine input/output pins, already all used
        // Future implementation is possible: wifi from the client may be used to transmit somewhere else theese values and functionality
        break;
      }
    case 'M':   // Motor step Command
      {
        sendMotorCmd(val);
        break;
      }
    case 'F':   // Change the timer frequency, the time between two steps
      {
        Serial.print(char(cmdArray[arrayIdxRead]));
        Serial.print(cmdArray[arrayIdxRead + 1]);
        Serial.println("y");
        switch (val)
        {
          case 1:   // Fast Speed
            {
              step_period = stepmin_period;
              noInterrupts();
              timer1_write((clockCyclesPerMicrosecond() / 16) * step_period);
              interrupts();
              break;
            }
          case 255:   // Cut Speed
            {
              step_period = stepmax_period;
              noInterrupts();
              timer1_write((clockCyclesPerMicrosecond() / 16) * step_period);
              interrupts();
              break;
            }
          default: // Pause (value in Jedicut) = pause in tenths of second
            //delay(100*(256-val));
            Serial.print(cmdArray[arrayIdxRead + 1]);
            Serial.println("F");
            timemark = (100 * (256 - val)) + millis();
        }
        break;
      }
    case 'U':   // Fast Speed LSB
      {
        stepmin_period = val;
        break;
      }
    case 'V':   // FAst Speed MSB
      {
        stepmin_period += 256 * val;
        break;
      }
    case 'X':   // CutSpeed LSB
      {
        stepmax_period = val;
        break;
      }
    case 'Y':   // Cut Speed MSB
      {
        stepmax_period += 256 * val;
        break;
      }
  }
}
//////////////////////////////////////////////////////
void flip() {
  if (isrActive) return;
  isrActive = true;
  if ((arrayIdxRead != arrayIdxWrite) || ovf)  // check if the buffer is empty
  {
    handleCommand();
    arrayIdxRead = arrayIdxRead + 2;
    if (arrayIdxRead == CMD_BUFFER_SIZE) arrayIdxRead = 0;
    komand_tot_count++;
    noInterrupts();
    cmdCounter = cmdCounter - 2;
    interrupts();

    if (ovf && (cmdCounter < CMD_BUFFER_SIZE / 2 - 50))
    {
      //buffer not full: continue sending commands
      ovf = false;
    }
  }
  else
  {
    //Serial.println("******underflow! break");
  }
  isrActive = false;
}



