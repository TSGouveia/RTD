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

//MUDAR ISTO, PRIMEIRO SLAVE = 1, SEGUNDO SLAVE = 2, ...
#define ESPSLAVENUMBER 1

//NÃO MUDAR ISTO
#define PORTMIN 1 + 7 * ESPSLAVENUMBER
#define PORTMAX 7 * (ESPSLAVENUMBER + 1)

void setup() {
  Serial.begin(115200);  //initialize serial communication at a 9600 baud rate
  start_ports();
  Serial.onReceive(Receive);
}
void loop() {
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

void start_ports() {
  ///////////////////////////////////////////////
  //Inicializar os portos
  //Input
  pinMode(ATUADOR_1, OUTPUT);
  pinMode(ATUADOR_2, OUTPUT);
  pinMode(ATUADOR_3, OUTPUT);
  pinMode(ATUADOR_4, OUTPUT);
  pinMode(ATUADOR_5, OUTPUT);
  pinMode(ATUADOR_6, OUTPUT);
  pinMode(ATUADOR_7, OUTPUT);

  //Output
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);
  pinMode(SENSOR_5, INPUT);
  pinMode(SENSOR_6, INPUT);
  pinMode(SENSOR_7, INPUT);
}

void Receive() {
  char incomingByte = 0;  // for incoming serial data
  char action = 0;
  char portIndexChar = 0;
  int portIndexInt = 0;
  char valueChar = 0;
  int valueInt = 0;

  while (Serial.available()) {
    incomingByte = (char)Serial.read();
    if (incomingByte == 'G' || incomingByte == 'S') { break; }
  }

  action = incomingByte;
  portIndexChar = Serial.read();
  portIndexInt = portIndexChar - '0';

  if (!(portIndexInt >= PORTMIN && portIndexInt <= PORTMAX)) { return; }
  portIndexInt -= 7 * ESPSLAVENUMBER;

  switch (action) {
    case 'G':
      Serial.write(sensor(portIndexInt) + '0');
      break;
    case 'S':
      valueChar = Serial.read();
      valueInt = valueChar - '0';
      atuador(portIndexInt, valueInt);
      break;
  }
}