#define PIN 3
#define PIN_potentiometer A0
#define PIN_ON_potention A1
#define PIN_PIR 2// пин DI
#define NUM_LEDS 24     // число диодов
#define sleep_time 1000 // миллисекунды 1c == 1000 Mc
#include "Adafruit_NeoPixel.h"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
class Photoresistor;
int32_t value_leds[NUM_LEDS];
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
    for (int i = 0; i < NUM_LEDS; i++ ) {     // всю ленту
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
    Photoresistor(int x, int y, int pin): pin(pin), x(x), y(y)
    {

    }
    int x = 0;
    int y = 0;
    int pin;
};