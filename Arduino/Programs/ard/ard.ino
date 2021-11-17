#define PIN 3
#define PIN_potentiometer A0
#define PIN_ON_potention A1
#define PIN_PIR 2// пин DI
#define NUM_LEDS 24     // число диодов
#define sleep_time 1000 // миллисекунды 1c == 1000 Mc
#define Quantity_TapeLed 4
#define Quantity_Led 6
#define Max_value_led 255

#include <math.h>
#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

double distance(int x1, int y1, int x2, int y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

class Photoresistor
{
  public:
    Photoresistor(int x, int y, int pin) : pin(pin), x(x), y(y)
    {
      value = analogRead(pin);
    }
    int x = 0;
    int y = 0;
    int pin;
    double k = 1; // y = kx
    int value = 0;
    int updateValue() // get and update value
    {
      value = analogRead(pin);
      return value;
    }
};

class Led
{
  public:
    Led(int num = 0, int x = 0, int y = 0, uint32_t value = 0) : num(num), x(x), y(y)
    {

    }
    int x, y;
    int num;
    uint32_t value;

};

class TapeLed
{
  public:

    TapeLed(int num = 0) : num(num), len_leds(0)
    {

    }
    int num;
    unsigned int len_leds = 0;
    Led* leds = new Led[len_leds];
    void addled(Led led)
    {

      Led* new_leds = new Led[len_leds + 1];
      for (unsigned int i = 0; i < len_leds; i++)
      {
        new_leds[i] = leds[i];
      }
      new_leds[len_leds] = led;
      leds = new_leds;
      len_leds++;
    }
};

Photoresistor photoresistors[4] = { Photoresistor(0, 0, 1), Photoresistor(10, 0, 2), Photoresistor(0, 10, 3), Photoresistor(10, 10, 4) };
int32_t value_leds[NUM_LEDS];
TapeLed tapeled[4];

void setup() {
  pinMode(PIN_ON_potention, INPUT);
  pinMode(PIN_potentiometer, INPUT);
  pinMode(PIN_PIR, INPUT);
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(200);        // яркость, от 0 до 255
  strip.clear();                                                    // очистить
  strip.show();                                                     // отправить на ленту
  for (int i = 0; i < NUM_LEDS; i++)
  {
    value_leds[i] = 0xffffff;
  }
  for (int i = 0; i < Quantity_TapeLed; i++)
  {
    tapeled[i] = TapeLed(i);
    if (i % 2 == 0)
    {
      for (int j = 0; j < Quantity_Led; j++)
      {
        tapeled[i].addled(Led(j, i, j)); // 1
      }
    }
    else
    {
      for (int j = Quantity_Led - 1; j >= 0; j--)
      {
        tapeled[i].addled(Led(j, i, Quantity_Led - j - 1)); // 2 одинаково с 1
      }
    }
  }

  for (int n = 0; n < sizeof(photoresistors) / sizeof(Photoresistor); n++)
  {
    //Led* max_dist_led = new Led(0, 0, 0);
    double max_dist = 0.0;
    for (int i = 0; i < sizeof(tapeled) / sizeof(TapeLed); i++)
    {
      for (int j = 0; j < tapeled[i].len_leds; j++)
      {
        double d = distance(photoresistors[n].x, photoresistors[n].y, tapeled[i].leds[j].x, tapeled[i].leds[j].y);

        if (max_dist < d)
        {
          max_dist = d;
          //max_dist_led = &tapeled[i].leds[j];
        }
      }
    }
    photoresistors[n].k = (-Max_value_led / max_dist);
    /*cout << "max_dist" << max_dist;
      cout << " k(ph) = " << photoresistors[n].k;
      cout << " x(ph) = " << photoresistors[n].x << " y(ph) = " << photoresistors[n].y;
      cout << " x(led) = " << max_dist_led->x << " y(led) = " << max_dist_led->y;
      cout << endl;*/
  }
}
bool move_t(); // Датчик движения True если обнаруженно движение
float lighting(int);
int time_move = millis();
int potentiometer_value = 0;
void loop() {

  if (analogRead(PIN_ON_potention) > 600) // Вкрючен ли потенциометр
  {
    Serial.println("Manual adjustment is enabled");
    for (int i = 0; i < sizeof(tapeled) / sizeof(TapeLed); i++)
    {
      //cout << "led.len = " << tapeled[i].len_leds << endl;
      for (unsigned int j = 0; j < tapeled[i].len_leds; j++)
      {
        //cout << "sum_value = " << sum_value << endl;
        tapeled[i].leds[j].value = (uint32_t)(floor(potentiometer_value / 4));
        //cout << "average value = " << tapeled[i].leds[j].value << endl;
      }
    }
    Serial.println(potentiometer_value);
  }
  else
  { // Автономный режим
    for (int i = 0; i < sizeof(tapeled) / sizeof(TapeLed); i++)
    {
      //cout << "led.len = " << tapeled[i].len_leds << endl;
      for (unsigned int j = 0; j < tapeled[i].len_leds; j++)
      {
        double sum_value = 0;
        double value = 0.0;
        for (int n = 0; n < sizeof(photoresistors) / sizeof(Photoresistor); n++)
        {
          value = Max_value_led + (photoresistors[n].k * distance(photoresistors[n].x, photoresistors[n].y, tapeled[i].leds[j].x, tapeled[i].leds[j].y));
          sum_value += value;
        }
        //cout << "sum_value = " << sum_value << endl;
        tapeled[i].leds[j].value = (uint32_t)(floor(sum_value / (sizeof(photoresistors) / sizeof(Photoresistor))));
        //cout << "average value = " << tapeled[i].leds[j].value << endl;
      }
    }
  }

  strip.clear();// очистить
  int k = 0;
  for (int i = 0; i < sizeof(tapeled) / sizeof(TapeLed); i++)
  {
    for (unsigned int j = 0; j < tapeled[i].len_leds; j++)
    {
      strip.setPixelColor(k + tapeled[i].leds[j].num, strip.Color(tapeled[i].leds[j].value, tapeled[i].leds[j].value, tapeled[i].leds[j].value)); 
    }
    k += tapeled[i].len_leds;
  }
  strip.setBrightness(255);        // яркость, от 0 до 255
  strip.show();// отправить на ленту
  Serial.println("----------------------------");
  //delay(1000);
}
bool move_t()
{
  int pirVal = digitalRead(PIN_PIR);
  Serial.println(digitalRead(PIN_PIR));
  return pirVal;
}
