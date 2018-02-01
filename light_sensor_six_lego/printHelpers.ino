
void printValues() {
  for (int i = 0; i < 6; i++) {
    Serial.print(values[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void printValuesNorm() {
  for (int i = 0; i < 6; i++) {
    Serial.print(values[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void printCalibrationValues() {
  char s[200];
  for (int i = 0; i < 6; i++) {
    sprintf(s, "valuesMin[%d] = %d\t\t valuesMax[%d] = %d \n", i, valuesMin[i], i, valuesMax[i]); 
    Serial.print(s);
  }
  Serial.flush();
}


