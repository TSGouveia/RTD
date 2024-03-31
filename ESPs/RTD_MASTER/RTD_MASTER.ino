#include <WiFi.h>
#include <WebServer.h>

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

#define MACHINEDELAY 1000

#define SKILL1 "C1SL1"
#define SKILL2 "SL1C2"
#define SKILL3 "C2C3"
#define SKILL4 "C3SL2"
#define SKILL5 "SL2C4"

#define SKILL6 "mill"
#define SKILL7 "drill"

#define SSID "Kit FMS"
#define PASSWORD "demo01FMS"

IPAddress local_IP(192, 168, 2, 69);  // Set your Static IP address
IPAddress gateway(192, 168, 1, 1);    // Set your Gateway IP address
IPAddress subnet(255, 255, 0, 0);

WebServer server(80);

int skillRequest = 0;

portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

////////////////////////////////
///////////////////////Handlers
////////////////////////////////

SemaphoreHandle_t sem_Milling;
SemaphoreHandle_t sem_Drilling;

SemaphoreHandle_t sem_mux_Slider1;
SemaphoreHandle_t sem_mux_Conveyor2;
SemaphoreHandle_t sem_mux_Conveyor3;
SemaphoreHandle_t sem_mux_Slider2;

SemaphoreHandle_t sem_Slider_1;
SemaphoreHandle_t sem_Slider_2;

SemaphoreHandle_t sem_C1SL1;
SemaphoreHandle_t sem_SL1C2;
SemaphoreHandle_t sem_C2C3;
SemaphoreHandle_t sem_C3SL2;
SemaphoreHandle_t sem_SL2C4;

//NOMENCLATURAS
// conv_1 = conveyor feed
// conv_2 = conveyor milling
// conv_3 = conveyor drilling
// conv_4 = conveyor swap

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Setup and Loop ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  //initialize serial communication at a 9600 baud rate
  startPorts();
  setupFreeRTOS();
  setupWiFi();
}
void loop() {
  server.handleClient();
  delay(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Sensor and Atuador /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Setup /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void startPorts() {
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
void setupFreeRTOS() {

  sem_mux_Slider1 = xSemaphoreCreateMutex();
  sem_mux_Conveyor2 = xSemaphoreCreateMutex();
  sem_mux_Conveyor3 = xSemaphoreCreateMutex();
  sem_mux_Slider2 = xSemaphoreCreateMutex();

  sem_Milling = xSemaphoreCreateCounting(100, 0);
  sem_Drilling = xSemaphoreCreateCounting(100, 0);

  sem_Slider_1 = xSemaphoreCreateCounting(100, 0);
  sem_Slider_2 = xSemaphoreCreateCounting(100, 0);

  sem_C1SL1 = xSemaphoreCreateCounting(100, 0);
  sem_SL1C2 = xSemaphoreCreateCounting(100, 0);
  sem_C2C3 = xSemaphoreCreateCounting(100, 0);
  sem_C3SL2 = xSemaphoreCreateCounting(100, 0);
  sem_SL2C4 = xSemaphoreCreateCounting(100, 0);

  xTaskCreate(Task_Receive_Skill, "Task_Receive_Skill", 1024, NULL, 0, NULL);

  xTaskCreate(Task_Conveyor1_Slider1, "Task_Conveyor1_Slider1", 2048, NULL, 0, NULL);
  xTaskCreate(Task_Slider1_Conveyor2, "Task_Slider1_Conveyor2", 2048, NULL, 0, NULL);
  xTaskCreate(Task_Conveyor2_Conveyor3, "Task_Conveyor2_Conveyor3", 2048, NULL, 0, NULL);
  xTaskCreate(Task_Conveyor3_Slider2, "Task_Conveyor3_Slider2", 2048, NULL, 0, NULL);
  xTaskCreate(Task_Slider2_Conveyor4, "Task_Slider2_Conveyor4", 2048, NULL, 0, NULL);

  xTaskCreate(Task_Milling_Machine, "Task_Milling_Machine", 1024, NULL, 0, NULL);
  xTaskCreate(Task_Drilling_Machine, "Task_Drilling_Machine", 1024, NULL, 0, NULL);

  xTaskCreate(Task_Slider_1, "Task_Slider_1", 1024, NULL, 0, NULL);
  xTaskCreate(Task_Slider_2, "Task_Slider_2", 1024, NULL, 0, NULL);
}
void setupWiFi() {
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Erro a configurar IP estático");
  }

  WiFi.setSleep(WIFI_PS_NONE);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/transporte/", HTTP_POST, handlePostTransporte);
  server.on("/estacao/", HTTP_POST, handlePostEstacao);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Get and Set ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// HTTP Requests /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void handlePostTransporte() {
  while (skillRequest != 0) {}

  String message = "";
  if (server.hasArg("skill")) {
    message = server.arg("skill");
  }
  if (message == SKILL1) {
    skillRequest = 1;
    server.send(200, "text/plain", "Skill " + message + " started");
  } else if (message == SKILL2) {
    skillRequest = 2;
    server.send(200, "text/plain", "Skill " + message + " started");
  } else if (message == SKILL3) {
    skillRequest = 3;
    server.send(200, "text/plain", "Skill " + message + " started");
  } else if (message == SKILL4) {
    skillRequest = 4;
    server.send(200, "text/plain", "Skill " + message + " started");
  } else if (message == SKILL5) {
    skillRequest = 5;
    server.send(200, "text/plain", "Skill " + message + " started");
  } else {
    server.send(200, "text/plain", "Skill " + message + " not found");
  }
}
void handlePostEstacao() {
  while (skillRequest != 0) {}

  String message = "";
  if (server.hasArg("skill")) {
    message = server.arg("skill");
  }
  if (message == SKILL6) {
    xSemaphoreGive(sem_Milling);
    server.send(200, "text/plain", "Skill " + message + " started");
  } else if (message == SKILL7) {
    xSemaphoreGive(sem_Drilling);
    server.send(200, "text/plain", "Skill " + message + " started");
  } else {
    server.send(200, "text/plain", "Skill " + message + " not found");
  }
}
void handleNotFound() {
  String message = "Erro ao executar skill\n\n";
  message += "Obtido:\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMetodo: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArgumentos: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "\nEsperado:\nURI: /transporte/\nMetodo: POST\nArgumentos: 1\n skill: C1SL1\n";
  message += "\nVerifica se o pedido é um POST";
  message += "\nVerifica se o pedido tem o seguinte formato:\n";
  message += "192.168.2.69:80/transporte/?skill=C1SL1\n";
  message += "\nSkills disponiveis:\n";
  message += " /transporte/\n  ";
  message += SKILL1;
  message += "\n  ";
  message += SKILL2;
  message += "\n  ";
  message += SKILL3;
  message += "\n  ";
  message += SKILL4;
  message += "\n  ";
  message += SKILL5;
  message += "\n ";

  message += "/estacao/\n  ";
  message += SKILL6;
  message += "\n  ";
  message += SKILL7;
  message += "\n";

  server.send(404, "text/plain", message);
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Receive Skill /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void Task_Receive_Skill(void* pvParameters) {
  while (1) {
    switch (skillRequest) {
      case 0:
        break;
      case 1:
        xSemaphoreGive(sem_C1SL1);
        break;
      case 2:
        xSemaphoreGive(sem_SL1C2);
        break;
      case 3:
        xSemaphoreGive(sem_C2C3);
        break;
      case 4:
        xSemaphoreGive(sem_C3SL2);
        break;
      case 5:
        xSemaphoreGive(sem_SL2C4);
        break;
      case 6:
        xSemaphoreGive(sem_Milling);
        break;
      case 7:
        xSemaphoreGive(sem_Drilling);
        break;
    }
    skillRequest = 0;
    vTaskDelay(1);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Conveyor and Sliders /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void Task_Conveyor1_Slider1(void* pvParameters) {
  while (1) {
    xSemaphoreTake(sem_C1SL1, portMAX_DELAY);
    xSemaphoreTake(sem_mux_Slider1, portMAX_DELAY);

    while (!SenseLoadingStation())
      continue;

    moveConveyorFeed();

    while (!SenseSlider1())
      continue;

    vTaskDelay(1000);
    stopConveyorFeed();

    xSemaphoreGive(sem_mux_Slider1);

    vTaskDelay(1);
  }
}
void Task_Slider1_Conveyor2(void* pvParameters) {
  while (1) {
    xSemaphoreTake(sem_SL1C2, portMAX_DELAY);
    xSemaphoreTake(sem_mux_Slider1, portMAX_DELAY);
    xSemaphoreTake(sem_mux_Conveyor2, portMAX_DELAY);

    moveConveyorMilling();

    xSemaphoreGive(sem_Slider_1);

    while (!SenseMillingMachine())  //Waits for a product to be in front of the Machine
      continue;

    stopConveyorMilling();

    xSemaphoreGive(sem_mux_Conveyor2);
    xSemaphoreGive(sem_mux_Slider1);

    vTaskDelay(1);
  }
}
void Task_Conveyor2_Conveyor3(void* pvParameters) {
  while (1) {
    xSemaphoreTake(sem_C2C3, portMAX_DELAY);
    xSemaphoreTake(sem_mux_Conveyor2, portMAX_DELAY);
    xSemaphoreTake(sem_mux_Conveyor3, portMAX_DELAY);

    moveConveyorMilling();
    moveConveyorDrilling();

    while (!SenseDrillingMachine())  //Waits for a product to be in front of the Machine
      continue;

    stopConveyorMilling();
    stopConveyorDrilling();

    xSemaphoreGive(sem_mux_Conveyor3);
    xSemaphoreGive(sem_mux_Conveyor2);

    vTaskDelay(1);
  }
}
void Task_Conveyor3_Slider2(void* pvParameters) {
  xSemaphoreTake(sem_C3SL2, portMAX_DELAY);
  xSemaphoreTake(sem_mux_Slider2, portMAX_DELAY);
  xSemaphoreTake(sem_mux_Conveyor3, portMAX_DELAY);

  moveConveyorDrilling();

  vTaskDelay(1000);

  stopConveyorDrilling();

  xSemaphoreGive(sem_mux_Conveyor3);
  xSemaphoreGive(sem_mux_Slider2);

  vTaskDelay(1);
}
void Task_Slider2_Conveyor4(void* pvParameters) {
  xSemaphoreTake(sem_SL2C4, portMAX_DELAY);
  xSemaphoreTake(sem_mux_Slider2, portMAX_DELAY);

  moveConveyorSwap();

  xSemaphoreGive(sem_Slider_2);

  while (!SenseConveyorSwap())  //Waits for a product to be in front of the Machine
    continue;

  stopConveyorSwap();

  xSemaphoreGive(sem_mux_Slider2);

  vTaskDelay(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// Machines /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void Task_Milling_Machine(void* pvParameters) {
  while (1) {
    xSemaphoreTake(sem_Milling, portMAX_DELAY);
    executeMillingMachine();
    vTaskDelay(MACHINEDELAY);
    stopMillingMachine();

    vTaskDelay(1);
  }
}
void Task_Drilling_Machine(void* pvParameters) {
  while (1) {
    xSemaphoreTake(sem_Drilling, portMAX_DELAY);
    executeDrillingMachine();
    vTaskDelay(MACHINEDELAY);
    stopDrillingMachine();

    vTaskDelay(1);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// Sliders //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void Task_Slider_1(void* pvParameters) {
  //gotoSlider1(0);
  while (true) {
    xSemaphoreTake(sem_Slider_1, portMAX_DELAY);
    gotoSlider1(1);
    gotoSlider1(0);

    vTaskDelay(1);
  }
}
void Task_Slider_2(void* pvParameters) {
  //gotoSlider2(0);
  while (true) {
    xSemaphoreTake(sem_Slider_2, portMAX_DELAY);
    gotoSlider2(1);
    gotoSlider2(0);

    vTaskDelay(1);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Conveyor Feed //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopConveyorFeed() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(5, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void moveConveyorFeed() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(5, 1);
  taskEXIT_CRITICAL(&spinlock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Conveyor Swap //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopConveyorSwap() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(10, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void moveConveyorSwap() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(10, 1);
  taskEXIT_CRITICAL(&spinlock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Conveyor Drilling //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopConveyorDrilling() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(8, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void moveConveyorDrilling() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(8, 1);
  taskEXIT_CRITICAL(&spinlock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Conveyor Milling ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopConveyorMilling() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(6, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void moveConveyorMilling() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(6, 1);
  taskEXIT_CRITICAL(&spinlock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Slider 1 ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopSlider1() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(1, 0);
  SetBitValue(2, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void moveSlider1Front() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(2, 0);
  SetBitValue(1, 1);
  taskEXIT_CRITICAL(&spinlock);
}
void moveSlider1Back() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(1, 0);
  SetBitValue(2, 1);
  taskEXIT_CRITICAL(&spinlock);
}
int getSlider1Pos() {
  taskENTER_CRITICAL(&spinlock);
  int I1 = GetBitValue(1);
  int I2 = GetBitValue(2);
  taskEXIT_CRITICAL(&spinlock);
  if (I2 == 1)
    return 0;  //if high level it's retracted
  else if (I1 == 1)
    return 1;      //if high level it's extended
  else return -1;  //if none are high level it's not on either ends
}
void gotoSlider1(int pos) {
  if (pos == 1) {
    moveSlider1Front();  //if pos == 1 then send the Slider 1 forward

    while (getSlider1Pos() != 1) {
      continue;
    }
  } else {
    moveSlider1Back();  //if pos == 0 then send the Slider 1 back

    while (getSlider1Pos() != 0) {
      continue;
    }
  }

  stopSlider1();  //when pos reached stop
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Slider 2 ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopSlider2() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(3, 0);
  SetBitValue(4, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void moveSlider2Front() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(4, 0);
  SetBitValue(3, 1);
  taskEXIT_CRITICAL(&spinlock);
}
void moveSlider2Back() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(3, 0);
  SetBitValue(4, 1);
  taskEXIT_CRITICAL(&spinlock);
}
int getSlider2Pos() {
  taskENTER_CRITICAL(&spinlock);
  int I3 = GetBitValue(3);
  int I4 = GetBitValue(4);
  taskEXIT_CRITICAL(&spinlock);
  if (I4 == 1)
    return 0;  //if high level it's retracted
  else if (I3 == 1)
    return 1;      //if high level it's extended
  else return -1;  //if none are high level it's not on either ends
}
void gotoSlider2(int pos) {
  if (pos == 1) {
    moveSlider2Front();  //if pos == 1 then send the Slider 1 forward

    while (getSlider2Pos() != 1) {
      continue;
    }
  } else {
    moveSlider2Back();  //if pos == 0 then send the Slider 1 back

    while (getSlider2Pos() != 0) {
      continue;
    }
  }

  stopSlider2();  //when pos reached stop
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Milling Machine /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopMillingMachine() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(7, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void executeMillingMachine() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(7, 1);
  taskEXIT_CRITICAL(&spinlock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Drilling Machine ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void stopDrillingMachine() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(9, 0);
  taskEXIT_CRITICAL(&spinlock);
}
void executeDrillingMachine() {
  taskENTER_CRITICAL(&spinlock);
  SetBitValue(9, 1);
  taskEXIT_CRITICAL(&spinlock);
}

//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Block Sensors //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
bool SenseLoadingStation() {
  taskENTER_CRITICAL(&spinlock);
  int bit = GetBitValue(7);
  taskEXIT_CRITICAL(&spinlock);
  return (bit == 1);
}
bool SenseDrillingMachine() {
  taskENTER_CRITICAL(&spinlock);
  int bit = GetBitValue(8);
  taskEXIT_CRITICAL(&spinlock);
  return (bit == 1);
}
bool SenseMillingMachine() {
  taskENTER_CRITICAL(&spinlock);
  int bit = GetBitValue(6);
  taskEXIT_CRITICAL(&spinlock);
  return (bit == 1);
}
bool SenseSlider1() {
  taskENTER_CRITICAL(&spinlock);
  int bit = GetBitValue(5);
  taskEXIT_CRITICAL(&spinlock);
  return (bit == 1);
}
bool SenseConveyorSwap() {
  taskENTER_CRITICAL(&spinlock);
  int bit = GetBitValue(9);
  taskEXIT_CRITICAL(&spinlock);
  return (bit == 1);
}