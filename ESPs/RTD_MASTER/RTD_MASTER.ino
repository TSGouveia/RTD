#include <Wire.h>

////////////////////////////////////////////
//ATUADORES
#define Q1 23  //
#define Q2 22  //
#define Q3 21  //
#define Q4 19  //
#define Q5 18  //
#define Q6 5   //
#define Q7 17  //

//SENSORES
#define I1 35  //Slider 1 Front
#define I2 25  //Slider 1 Rear
#define I3 26  //Slider 2 Front
#define I4 27  //Slider 2 Rear
#define I5 2   //Slider 1 Light
#define I6 4   //Milling Machine Light
#define I7 16  //Loading Station Light

#define SLAVE_ADDRESS 0x08  // Address of the slave device

void setup() {
  Wire.begin();  // Initialize I2C communication
  Serial.begin(9600);
  inicializar_portos();
}

void loop() {
}

void inicializar_portos() {

  ///////////////////////////////////////////////
  //Inicializar os portos
  //Input
  pinMode(Q1, OUTPUT);
  pinMode(Q2, OUTPUT);
  pinMode(Q3, OUTPUT);
  pinMode(Q4, OUTPUT);
  pinMode(Q5, OUTPUT);
  pinMode(Q6, OUTPUT);
  pinMode(Q7, OUTPUT);

  //Output
  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  pinMode(I3, INPUT);
  pinMode(I4, INPUT);
  pinMode(I5, INPUT);
  pinMode(I6, INPUT);
  pinMode(I7, INPUT);
}

int GetBitValue(int port_Index) {
  if (port_Index <= 7) {
    return sensor(port_Index);
  } else {
    char message[2];
    message[0] = 'G';
    message[1] = port_Index + '0';
    return send_I2C(message);
  }
}
void SetBitValue(int port_Index, int value) {
  if (port_Index <= 7) {
    atuador(port_Index, value);
  } else {
    char message[3];
    message[0] = 'S';
    message[1] = port_Index + '0';
    message[2] = value + '0';
    send_I2C(message);
  }
}

int sensor(int portIndex) {
  switch (portIndex) {
    case 1:
      return digitalRead(I1);
      break;
    case 2:
      return digitalRead(I2);
      break;
    case 3:
      return digitalRead(I3);
      break;
    case 4:
      return digitalRead(I4);
      break;
    case 5:
      return digitalRead(I5);
      break;
    case 6:
      return digitalRead(I6);
      break;
    case 7:
      return digitalRead(I7);
      break;
  }
}
void atuador(int portIndex, int high) {
  switch (portIndex) {
    case 1:
      digitalWrite(Q1, high);
      break;
    case 2:
      digitalWrite(Q2, high);
      break;
    case 3:
      digitalWrite(Q3, high);
      break;
    case 4:
      digitalWrite(Q4, high);
      break;
    case 5:
      digitalWrite(Q5, high);
      break;
    case 6:
      digitalWrite(Q6, high);
      break;
    case 7:
      digitalWrite(Q7, high);
      break;
  }
}

int send_I2C(char* message) {
  int c = -1;
  //Serial.println(message);
  Wire.beginTransmission(SLAVE_ADDRESS);  // Start communication with the slave device
  Wire.write(message);                    // Send data to the slave
  Wire.endTransmission();                 // End transmission

  if (message[0] == 'G') {
    // Request data from the slave
    Wire.requestFrom(SLAVE_ADDRESS, 1);  // Request 6 bytes of data from slave
    while (Wire.available()) {           // Read the data sent by slave
      c = Wire.read();
      //Serial.print(c);
    }
    //Serial.println();
  }
  return c;
}