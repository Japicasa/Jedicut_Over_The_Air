void send_speed_settings() {
cmdArray[0] ='U';
cmdArray[1] = lowByte(stepmin_period);
cmdArray[2] = 'V';
cmdArray[3] = highByte(stepmin_period);
cmdArray[4] = 'X';
cmdArray[5] =  lowByte(stepmax_period);
cmdArray[6] = 'Y';
cmdArray[7] = highByte(stepmax_period);
arrayIdxWrite =  8;
cmdCounter  = 8;
}

