#include <Arduino.h>
#include <WiFi.h>

// Current readings
typedef struct readings{
  float voltage;
  float current;
} readings;

// Transmit Packet
typedef struct transmit{
  char dock;
  uint8_t slip;
  readings measurements;
  uint32_t timeElapsed;
  float energy;
} transmit;

// Global Declarations
transmit packet;
char* recBuffer;
char* sendBuffer;

WiFiClient client;

uint64_t timer = 0;

float readVoltage(){
  return 3.3;
}

float readCurrent(){
  return 0.32;
}

void setup() {
  Serial.begin(9600);

  Serial.println("Connecting to WiFi..");
  WiFi.mode(WIFI_STA);
  WiFi.begin("BsquaredGoogle", "6049262300");
  while(WiFi.status() != WL_CONNECTED);
  Serial.print("WiFi Connected:");
  Serial.println(WiFi.localIP());
  client.connect("192.168.86.43", 5566);

}

float usedEnergy = 0;

void loop() {

  usedEnergy +=readVoltage()*readCurrent()*(20/(1000*360));

  packet = {
    'A',1,
    {
      120,
      4
    },
    200,
    (120*4)
  };

  if(timer > 4000){
    Serial.println("Sending...");
    sendBuffer = (char*)malloc(sizeof(transmit));
    memcpy(sendBuffer, (char*)&packet, sizeof(transmit));
    client.connect("192.168.86.43", 5566);
    client.write(sendBuffer, sizeof(transmit));
    Serial.println("Data Sent\nConfirming..");
    delay(10);
    recBuffer = (char*)calloc(1, sizeof(char[10]));
    for (int i = 0; i < sizeof(char[2]); i++)
    {
      if(client.available()){
        recBuffer[i] = client.read();
      }
      else{
        delay(10);
        i--;
      }
    }
    Serial.print("Confirmation Recieved: ");
    Serial.println(recBuffer);
    client.stop();
    free(recBuffer);
    free(sendBuffer);
    timer = 0;
    usedEnergy = 0;
  }

  delay(20);
  timer+=20;
}