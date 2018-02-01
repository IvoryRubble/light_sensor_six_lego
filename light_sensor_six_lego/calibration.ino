
void calibration() {
  Serial.println("start calibration");
  Serial.flush();

  for (int i = 0; i < 6; i++) {
    valuesMin[i] = 1023;
    valuesMax[i] = 0; 
  }
  
  unsigned long currentTime = millis();
    
  unsigned long startTime = currentTime;
  
  const unsigned long ledOnTime = 125;
  const unsigned long ledOffTime = 125;
  unsigned long lastLedSwitchTime = currentTime;
  int ledState = LOW;
  digitalWrite(LED, LOW);
                                     
  while (currentTime - startTime < CALIBRATION_TIME) {
    getValues();
    for (int i = 0; i < 6; i++) {
      if (values[i] < valuesMin[i]) {
        valuesMin[i] = values[i];
      }
      if (values[i] > valuesMax[i]) {
        valuesMax[i] = values[i];
      }
    }

    if (ledState == LOW && currentTime - lastLedSwitchTime > ledOffTime) {
      ledState = HIGH;
      digitalWrite(LED, ledState);
      lastLedSwitchTime = currentTime;
    }
    if (ledState == HIGH && currentTime - lastLedSwitchTime > ledOnTime) {
      ledState = LOW;
      digitalWrite(LED, ledState);
      lastLedSwitchTime = currentTime;
    }

    currentTime = millis();
  }

  digitalWrite(LED, LOW);
  delay(200);

  saveCalibrationValues();
  printCalibrationValues();
}

void loadCalibrationValues() {
  EEPROM.get(valuesMinAddress, valuesMin);
  EEPROM.get(valuesMaxAddress, valuesMax);
}

void saveCalibrationValues() {
  EEPROM.put(valuesMinAddress, valuesMin);
  EEPROM.put(valuesMaxAddress, valuesMax);
}

