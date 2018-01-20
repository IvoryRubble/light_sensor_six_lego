
void calibration() {
  Serial.println("start calibration");
  Serial.flush();
  unsigned long startTime = millis();
                                   
  while (millis() - startTime < CALIBRATION_TIME) {
    getValues();
    for (int i = 0; i < 6; i++) {
      if (values[i] < valuesMin[i]) {
        valuesMin[i] = values[i];
      }
      if (values[i] > valuesMax[i]) {
        valuesMax[i] = values[i];
      }
    }
  }
  
  char s[50];
  for (int i = 0; i < 6; i++) {
    sprintf(s, "valuesMin[%d] = %d\t\t valuesmax[%d] = %d", i, valuesMin[i], i, valuesMax[i]); 
  }
  Serial.print(s);
  Serial.flush();
}
