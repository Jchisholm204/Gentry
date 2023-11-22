#include <Arduino.h>
#include <WiFi.h>

#define curr 35
#define volt 34

WiFiClient client;

long double zeroPoint = 0;

typedef struct data{
  float voltage;
  float current;
} data;

void sendData(int id, data _data, WiFiClient *Client){
  uint8_t voltage = _data.voltage*100;
  uint8_t current = _data.current*100;
  Client->write(id);
  Client->write(voltage);
  Client->write(current);
}

void setup() {
  Serial.begin(9600);
  pinMode(curr, INPUT);
  pinMode(volt, INPUT);

  Serial.println("Connecting to WiFi..");
  WiFi.mode(WIFI_STA);
  WiFi.begin("BsquaredGoogle", "6049262300");
  while(WiFi.status() != WL_CONNECTED);
  Serial.print("WiFi Connected:");
  Serial.println(WiFi.localIP());
  //Serial.println("Connecting To Server");
  client.connect("192.168.86.43", 5566);
  //while(!client.connected());
  //Serial.println("Connected to Client");
  //Serial.println(client.connected());
  //Serial.println(client.available());
  //Serial.println(client.availableForWrite());

  Serial.println("Calibrating Current Sensor...\n Ensure that no current flows through the sensor at this moment");

  for (int i = 0; i < 10000; i++)
  {
    zeroPoint += analogRead(curr)*3.3/4095;
    delay(1);
  }
  zeroPoint /= 10000;
  
  Serial.println("Done!");
  Serial.print("Zero = ");
  Serial.println((double)zeroPoint);
  //Serial.println("Zero point for this sensor = " + zero);
}

void loop() {
  // Read current from sensor
  //float I = sensor.getCurrentDC();
  
  // Send it to serial
  //Serial.println(String("I = ") + I + " A");
  
  // Wait a second before the new measurement
  long double average = 0;
  float Vavg = 0;
  for(int i = 0; i < 1000; i++) {
    average += (analogRead(curr)*3.3/4096);//this is
    Vavg += analogRead(volt)*3.3/(4095);
    //for the 5A mode, if 20A or 30A mode, need to modify this formula to 
    //(.19 * analogRead(A0) -25) for 20A mode and 
    //(.044 * analogRead(A0) -3.78) for 30A mode
    delay(1);
  }
  Vavg/=1000;
  Vavg*=(2000+330)/330;
  //Vavg*=2;

  average/=1000;
  average -= zeroPoint;
  average*=1000/66;

  client.connect("192.168.86.43", 5566);
  client.write((const byte*) &Vavg, sizeof(Vavg));
  client.stop();
}