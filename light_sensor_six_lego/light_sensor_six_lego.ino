#include <EEPROM.h>
#include <Wire.h>

const int SLAVE_ADDRESS = 0x04;
const unsigned long CALIBRATION_TIME = 3000;
const int CALIBRATION_BUTTON = 2;
const int LED = 5;
const int SENSORS[6] = {A0, A1, A2, A3, A6, A7};

uint8_t message[8] = {0};

int values[6] = {0};
float valuesNorm[6] = {0}; //form 0 to 1
float linePosition = 0; //from -1 to 1
float polePosition = 0; //from -1 to 1 
                        //polePosition >= 0 => on black pole
                        //polePosition < 0 => on white pole
                        
int valuesMin[6];
const int valuesMinAddress = 0;
int valuesMax[6];
const int valuesMaxAddress = valuesMinAddress + sizeof(valuesMax);
const float weights[6] = {-0.5, -0.33, -0.167, 0.167, 0.33, 0.5};

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  pinMode(CALIBRATION_BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  loadCalibrationValues();
  
  Serial.begin(9600); 
  printCalibrationValues();
  Serial.println();
  Serial.print("SlaveAddress = 0x");
  Serial.println(SLAVE_ADDRESS, HEX);
  Serial.println();
  Serial.println("Ready");
  Serial.println();
}

void loop() {
  getValues();
  calcNormValues();
  calcLinePosition();
  calcPolePosition();

  makeMessage();
  
  printValues();
  //printValuesNorm();
  //Serial.println(linePosition);
  //Serial.println(polePosition);

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

  message[0] = map(polePosition, -1, 1, -127, 127);
}

void getValues() {
  for (int i = 0; i < 6; i++) {
    values[i] = analogRead(SENSORS[i]);
  }
}

void calcNormValues() {
  for (int i = 0; i < 6; i++) {
    //valuesNorm[i] from 0 to 1
    valuesNorm[i] = (float)(values[i] - valuesMin[i]) / (float)(valuesMax[i] - valuesMin[i]);
    valuesNorm[i] = constrain(valuesNorm[i], 0.0f, 1.0f); 
  }
}

void calcLinePosition() {
  linePosition = 0;
  for (int i = 0; i < 6; i++) {
    linePosition += weights[i] * valuesNorm[i];
  }
  linePosition = constrain(linePosition, -1.0f, 1.0f);
}

void calcPolePosition() {
  polePosition = 0;
  for (int i = 0; i < 6; i++) {
    polePosition += valuesNorm[i] * 2 - 1;
  }
  polePosition /= 6;
  polePosition = constrain(polePosition, -1.0f, 1.0f);
}

void receiveData(int byteCount) {
  byteCount = byteCount; //just suppress unsused parameter warning 
  //nothing
}

void sendData() {
  Wire.write(message, 8);
}
