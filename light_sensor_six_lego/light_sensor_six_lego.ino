#include <Wire.h>

const int SLAVE_ADDRESS = 0x04;
const unsigned long CALIBRATION_TIME = 3000;
const int CALIBRATION_BUTTON = 2;
const int LED = 5;

uint8_t message[8] = {0};

int values[6] = {0};
float valuesNorm[6] = {0};
float linePosition = 0;

int valuesMin[6] = {0, 0, 0, 0, 0, 0};
int valuesMax[6] = {1023, 1023, 1023, 1023, 1023, 1023};
float weights[6] = {-0.5, -0.33, -0.167, 0.167, 0.33, 0.5};

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  pinMode(CALIBRATION_BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  Serial.begin(9600); 
  Serial.println("Ready");
  Serial.print("SlaveAddress = 0x");
  Serial.println(SLAVE_ADDRESS, HEX);
}

void loop() {
  getValues();
  calcNormValues();
  calcLinePosition();

  makeMessage();
  
  printValues();
  //printValuesNorm();
  //Serial.println(linePosition);

  if (digitalRead(CALIBRATION_BUTTON) == LOW) {
    calibration();
  }
  //delay(1000);
}

void makeMessage() {
  message[7] = map(valuesNorm[0], 0, 1, -127, 127);
  message[6] = map(valuesNorm[1], 0, 1, -127, 127);
  message[5] = map(valuesNorm[2], 0, 1, -127, 127);
  message[4] = map(valuesNorm[3], 0, 1, -127, 127);
  message[3] = map(valuesNorm[4], 0, 1, -127, 127);
  message[2] = map(valuesNorm[5], 0, 1, -127, 127);

  message[1] = map(linePosition, -1, 1, -127, 127);
}

void getValues() {
  values[0] = analogRead(A0);
  values[1] = analogRead(A1);
  values[2] = analogRead(A2);
  values[3] = analogRead(A3);
  values[4] = analogRead(A4);
  values[5] = analogRead(A5);
}

void calcNormValues() {
  for (int i = 0; i < 6; i++) {
    //valuesNorm[i] from 0 to 1
    valuesNorm[i] = (float)(values[i] - valuesMin[i]) / (float)(valuesMax[i] - valuesMin[i]);
    valuesNorm[i] = constrain(valuesNorm[i], 0.0f, 1.0f); 
  }
}

void calcLinePosition() {
  for (int i = 0; i < 6; i++) {
    linePosition += weights[i] * valuesNorm[i];
  }
  linePosition = constrain(linePosition, -1.0f, 1.0f);
}

void printValues() {
  char s[50];
  for (int i = 0; i < 6; i++) {
    sprintf(s, "%d\t", values[i]);
    Serial.println(s);
  }
}

void printValuesNorm() {
  char s[50];
  for (int i = 0; i < 6; i++) {
    sprintf(s, "%g\t", (double)valuesNorm[i]);
    Serial.println(s);
  }
}

void receiveData(int byteCount) {
  byteCount = byteCount; //just suppress unsused parameter warning 
  //nothing
}

void sendData() {
  Wire.write(message, 8);
}
