
void printValues() {
  Serial.print("raw values:\t");
  for (int i = 0; i < 6; i++) {
    Serial.print(values[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void printValuesNorm() {
  Serial.print("norm values:\t");
  for (int i = 0; i < 6; i++) {
    Serial.print(valuesNorm[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void printCalibrationValues() {
  char s[300];
  for (int i = 0; i < 6; i++) {
    sprintf(s, "valuesMin[%d] = %d\t\t valuesMax[%d] = %d \n", i, valuesMin[i], i, valuesMax[i]); 
    Serial.print(s);
  }
  Serial.flush();
}

void printLinePosition() {
  Serial.print("line position:\t");
  Serial.println(linePosition);
}

void printPolePosition() {
  Serial.print("pole position:\t");
  Serial.println(polePosition);
}

void printMessage() {
  Serial.print("message:\t");
  for (int i = 0; i < 8; i++) {
    Serial.print((int8_t)message[i]);
    Serial.print("\t");
  }
  Serial.println();
}


