#include <Wire.h>

////////////////////////////////////////////
#define Q1 23  //
#define Q2 22  //
#define Q3 21  //
#define Q4 19  //
#define Q5 18  //
#define Q6 5   //
#define Q7 17  //

#define I1 35  //Slider 1 Front
#define I2 25  //Slider 1 Rear
#define I3 26  //Slider 2 Front
#define I4 27  //Slider 2 Rear
#define I5 2   //Slider 1 Light
#define I6 4   //Milling Machine Light
#define I7 16  //Loading Station Light

#define SLAVE_ADDRESS 0x08  // Address of the slave device

int debugVar = 0;

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

int send_I2C(char* message) {
  int c = -1;
  //Serial.println(message);
  Wire.beginTransmission(SLAVE_ADDRESS);  // Start communication with the slave device
  Wire.write(message);                    // Send data to the slave
  Wire.endTransmission();                 // End transmission

  // Request data from the slave
  Wire.requestFrom(SLAVE_ADDRESS, 1);  // Request 6 bytes of data from slave
  while (Wire.available()) {  // Read the data sent by slave
    c = Wire.read();
    //Serial.print(c);
  }
  //Serial.println();
  return c;
}