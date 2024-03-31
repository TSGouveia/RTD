#define ATUADOR_1 23
#define ATUADOR_2 22
#define ATUADOR_3 21
#define ATUADOR_4 19
#define ATUADOR_5 18
#define ATUADOR_6 5
#define ATUADOR_7 17

#define SENSOR_1 35
#define SENSOR_2 25
#define SENSOR_3 26
#define SENSOR_4 27
#define SENSOR_5 2
#define SENSOR_6 4
#define SENSOR_7 16

void setup() {
  Serial.begin(115200);  //initialize serial communication at a 9600 baud rate
  pinMode(2, OUTPUT);
}
void loop() {
  digitalWrite(2, GetBitValue(8));
}
int sensor(int portIndex) {
  switch (portIndex) {
    case 1:
      return digitalRead(SENSOR_1);
      break;
    case 2:
      return digitalRead(SENSOR_2);
      break;
    case 3:
      return digitalRead(SENSOR_3);
      break;
    case 4:
      return digitalRead(SENSOR_4);
      break;
    case 5:
      return digitalRead(SENSOR_5);
      break;
    case 6:
      return digitalRead(SENSOR_6);
      break;
    case 7:
      return digitalRead(SENSOR_7);
      break;
  }
}
void atuador(int portIndex, int high) {
  switch (portIndex) {
    case 1:
      digitalWrite(ATUADOR_1, high);
      break;
    case 2:
      digitalWrite(ATUADOR_2, high);
      break;
    case 3:
      digitalWrite(ATUADOR_3, high);
      break;
    case 4:
      digitalWrite(ATUADOR_4, high);
      break;
    case 5:
      digitalWrite(ATUADOR_5, high);
      break;
    case 6:
      digitalWrite(ATUADOR_6, high);
      break;
    case 7:
      digitalWrite(ATUADOR_7, high);
      break;
  }
}

int GetBitValue(int portIndex) { 
  if (portIndex <= 7) {
    return sensor(portIndex);
  }
  char incomingByte;
  char message[3];


  message[0] = 'G';
  message[1] = portIndex + '0';
  message[2] = '\0';

  Serial.write(message);

  while (!Serial.available()) {}

  while (Serial.available()) {
    incomingByte = (char)Serial.read();
    if (incomingByte == '0' || incomingByte == '1') { break; }
  }

  return incomingByte - '0';
}
void SetBitValue(int portIndex, int value) {
  if (portIndex <= 7) {
    return atuador(portIndex, value);
  }
  char message[4];

  message[0] = 'S';
  message[1] = portIndex + '0';
  message[2] = value + '0';
  message[3] = '\0';

  Serial.write(message);
}