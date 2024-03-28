#include <Wire.h>

////////////////////////////////////////////
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

#define MY_ADDRESS 0x08

typedef struct {
  char action;
  int port_index;
  int value;
} request_t;

request_t request;

//ARDUINO
void setup() {
  Serial.begin(9600);
  Serial.println();
  start_ports();
  setup_I2C();

  Serial.println("Slave Ready");
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

//SETUP
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
void setup_I2C() {
  Wire.begin(MY_ADDRESS);       // Initialize I2C communication as a slave
  Wire.onReceive(receiveData);  // Register the receive event handler
  Wire.onRequest(sendData);     // Register the request event handler
}

//I2C CALLBACKS
void receiveData(int _) {

  request.value = -1;  //Colocar valor por default, porque se for Get, este valor vai ser algo estranho

  for (int dataIndex = 0; Wire.available(); dataIndex++) {
    switch (dataIndex) {
      case 0:
        request.action = Wire.read();
        break;
      case 1:
        request.port_index = Wire.read() - '0';
        break;
      case 2:
        request.value = Wire.read() - '0';
        break;
      default:
        break;
    }
  }
}
void sendData() {
  int valToSend;

  switch (request.action) {
    //GET
    case 'G':
      valToSend = sensor(request.port_index);
      Wire.write(valToSend);
      break;

    //SET
    case 'S':
      atuador(request.port_index, request.value);
      break;
  }
  //Serial.printf("To message %c%d, I responded with %d\n",request.action,request.port_index,valToSend);
}