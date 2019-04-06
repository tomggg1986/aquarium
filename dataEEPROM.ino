//methods to read and save light start time
void saveLightOnTime(int hours, int minutes, int seconds) {
  int lightOnAddr = 0;
  EEPROM.write(lightOnAddr, hours);
  lightOnAddr += sizeof(hours);

  EEPROM.write(lightOnAddr, minutes);
  lightOnAddr += sizeof(minutes);
  EEPROM.write(lightOnAddr, seconds);
}

void readLightOnTime(int lightOnTime[]) {
  int lightOnAddr = 0;
  lightOnTime[0] = EEPROM.read(lightOnAddr);
  lightOnAddr += sizeof(lightOnTime[0]);

  lightOnTime[1] = EEPROM.read(lightOnAddr);
  lightOnAddr += sizeof(lightOnTime[1]);

  lightOnTime[2] = EEPROM.read(lightOnAddr);
  return  lightOnTime;
}
