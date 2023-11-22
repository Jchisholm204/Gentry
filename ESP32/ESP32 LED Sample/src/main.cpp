#include <Arduino.h>
#include <math.h>

class led
{
private:
  struct pins{
    int r, g, b;
  } pins;
public:
  /**
   * @brief Initialize a new LED
   * 
   * @param r Red Pin
   * @param g green Pin
   * @param b Blue Pin
   */
  void init(int r, int g, int b);
  /**
   * @brief Set the LED's Color
   * 
   * @param r red
   * @param g green
   * @param b blue
   */
  void setColor(int r, int g, int b);
  
  void cycle(int delay);
};

void led::init(int r, int g, int b)
{
  pins.r = r;
  pins.g = g;
  pins.b = b; 
  pinMode(pins.r, OUTPUT);
  pinMode(pins.g, OUTPUT);
  pinMode(pins.b, OUTPUT);

}

void led::setColor(int r, int g, int b){
  analogWrite(pins.r, r);
  analogWrite(pins.g, g);
  analogWrite(pins.b, b);
}

void led::cycle(int _delay){
  unsigned int rgb[3] = {255, 0, 0};

  for (int d = 0; d < 3; d++)
  {
    int i  = d == 2 ? 0 : d + 1;
    for (int j = 0; j < 255; j++)
    {
      rgb[d]--;
      rgb[i]++;
      setColor(rgb[0], rgb[1], rgb[2]);
      //Serial.printf("i: %d r: %d g: %d b: %d\n", i, rgb[0], rgb[1], rgb[2]);
      delay(_delay);
    }
    
  }
}

led light;


void setup() {
  Serial.begin(9600);
  light.init(33, 25, 26);
  Serial.println("Init");
  light.setColor(255, 255, 255);
}

void loop() {
  light.cycle(20);
}