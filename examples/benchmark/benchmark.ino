#include <Arduino.h>
//#include <math.h>
#include "/home/mikko/Documents/code/Subwoofer-Leds/scr/SubEffects.h"

SubEffects effect(0, 6, 100);       // effect( subwoofer analog pin , led data pin , led count )


void printMultiple(char a, uint16_t count) {      // Prints the given char number of times
  for (uint16_t i = 0; i < count; i++)
  {
    Serial.print(a);
  }
}

void setup() {
  Serial.begin(115200);

  while (!Serial)
  {
    // Do nothing
  }

  delay(1000);
}

void loop() {

  Serial.println(F("\nSubEffects"));
  printMultiple(' ', 4);
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print(F("| "));
    Serial.print( pow(2, i) );
    Serial.print(F("   |"));
  }
  Serial.println();

  effect.SetFrequency(700);
  printMultiple(' ', 4);
  uint32_t timeMicros;
  for (uint8_t i = 0; i < 15; i++)
  {
    Serial.print(F("| "));
    delay(500);
    timeMicros = micros();
    for (uint16_t a = 0; a < 200; a++)
    {
      effect.Benchmark();
    }
    float time = micros() - timeMicros;
    if (time > 1000)
    {
      time /= 1000;
      Serial.print(time, 2);
      Serial.print(F("ms"));
    } else
    {
      Serial.print(time);
      Serial.print(F("μs"));
    }
    float power;
    power = round(pow(2, i));
    if (!effect.SetSampleSize(power))
    {
      Serial.print(F("X"));
    }
    Serial.print(F(" |"));
  }
  Serial.println();
  Serial.println("Benchmark Completed");
  while (1)
  {
    delay(400);
  }
}
