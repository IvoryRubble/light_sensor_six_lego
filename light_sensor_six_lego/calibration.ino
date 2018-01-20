
void calibration() {
  Serial.println("start calibration");
  Serial.flush();

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
  
  char s[50];
  for (int i = 0; i < 6; i++) {
    sprintf(s, "valuesMin[%d] = %d\t\t valuesmax[%d] = %d \n", i, valuesMin[i], i, valuesMax[i]); 
  }
  Serial.print(s);
  Serial.flush();
}


