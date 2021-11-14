#define PIN 3
#define PIN_potentiometer A0
#define PIN_ON_potention A1
#define PIN_PIR 2// пин DI
#define NUM_LEDS 24     // число диодов
#define sleep_time 1000 // миллисекунды 1c == 1000 Mc
#define Quantity_TapeLed 4
#define Quantity_Led 6
#include <Vector.h>
#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
class Photoresistor;
class TapeLed;
class Led;
int32_t value_leds[NUM_LEDS];
Photoresistor photoresistors[4] = {Photoresistor(0, 0, A1), Photoresistor(0, 0, A2), Photoresistor(0, 0, A3), Photoresistor(0, 0, A4)};
TapeLed tapeled[quantity_TapeLed];
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
		value_leds[i] = strip.Color(255, 255, 255);
	}
    bool flag = True;
    for (int i = 0; i < Quantity_TapeLed; i++)
    {
        tapeled[i] = TapeLed(i)
        if (flag)
        {
            for (int j = 0;j < Quantity_Led; i++)
            {
                tapeled[i].addled(Led(j));
            }
        }
        else
        {
            for (int j = Quantity_Led - 1;j <= 0; i--)
            {

            }
        }
        flag != flag;
        
    }
}
bool move_t(); // Датчик движения True если обнаруженно движение
float lighting(int);
int time_move = millis();
int potentiometer_value = 0;
float a = 0.0;
void loop() {

	Serial.println("----------------------------");
	potentiometer_value = analogRead(PIN_potentiometer);
	Serial.print(analogRead(A2));
	Serial.print(' ');
	Serial.print(analogRead(A3));
	Serial.print(' ');
	Serial.print(analogRead(A4));
	Serial.print(' ');
	Serial.println(analogRead(A5));
	Serial.print("Potentiometer_value = ");
	Serial.println(potentiometer_value);
	Serial.print("PIN_ON_potention = ");
	Serial.println(PIN_ON_potention);

	if (analogRead(PIN_ON_potention) > 600) // Вкрючен ли потенциометр
	{
		Serial.println("Manual adjustment is enabled");
		a = potentiometer_value / 4;
		Serial.println(potentiometer_value);
	}
	else
	{ // Автономный режим
		a = ((2720 - (analogRead(A5) + analogRead(A4) + analogRead(A3) + analogRead(A2))) / 10.6);
		if (move_t())
		{
			time_move = millis();
			Serial.print("New time_move = ");
			Serial.println(time_move);
		}
		else
		{
			if (millis() - time_move >= sleep_time)
			{
				Serial.println("!sleep!");
				a = 0.0;
			}
			else
			{
				Serial.println("New time_move = old time_move, but not move");
			}
		}
	}

	Serial.println(a);

	strip.clear();                                                    // очистить
	for (int i = 0; i < NUM_LEDS; i++) {     // всю ленту
		strip.setPixelColor(i, value_leds[i]);         // залить чёрным
	}
	strip.setBrightness((int)(a));        // яркость, от 0 до 255
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
	Led(int num, uint32_t color = 0xffffff) : num(num), color(color)
	{

	}
	int num;
	uint32_t color;

};

class TapeLed
{
public:
  vector<Led> leds;
	TapeLed(int num) : num(num)
	{

	}
	int num;
	
	void addled(Led led)
	{
		leds.push_back(led);
	}
};
