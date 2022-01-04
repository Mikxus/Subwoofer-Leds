#include <Arduino.h>
//#include <math.h>
#include "D:\Code\Github\Subwoofer-Leds\scr\SubEffects.h"

SubEffects effect(0, 6, 100);       // effect( subwoofer analog pin , led data pin , led count )
void fft_double_benchmark();

void printMultiple(char a, uint16_t count) {      // Prints the given char number of times
  for (uint16_t i = 0; i < count; i++)
  {
    Serial.print(a);
    Serial.flush();
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

  fft_double_benchmark();
  /*
  printMultiple(' ', 4);
  for (uint8_t i = 0; i < 15; i++)
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
  */
  Serial.println();
  Serial.println("Benchmark Completed");
  while (1)
  {
    delay(400);
  }
}

void fft_double_benchmark()
{

  Serial.println(F("FFT using Floating point math Benchmark"));

  Serial.println(F("FFT | Bin Size | Time  | FPS approx |"));
  Serial.flush();
  uint8_t iteration;
  effect.SetFrequency(700);
  uint16_t power = 1;

  while (effect.SetSampleSize(power))
  {
    printMultiple(' ', 4);
    Serial.print(F("|"));
    uint8_t spaces = 0;

    if (power >= 10000 ) spaces = 3;
    else if ( power >= 1000 ) spaces = 4;
    else if ( power >= 100 ) spaces = 5;
    else if ( power >= 10 ) spaces = 6;
    else spaces = 7;

    Serial.print(power);
    printMultiple(' ', spaces);
    Serial.print(F("| "));

    float binCalcTime = micros();
    effect.Benchmark();
    binCalcTime = micros() - binCalcTime;

    if ( binCalcTime >= 1000 ) 
    {
      binCalcTime /= 1000;
      spaces = 2;
    } else if ( binCalcTime >= 100 ) spaces = 3;
    else if ( binCalcTime >= 10 ) spaces = 4;
    else spaces = 5;

    Serial.print(binCalcTime);
    printMultiple(' ', spaces);
    Serial.println(F("|"));

    iteration += 1;    
    power = round(pow( 2 , iteration ));
    Serial.flush();
  }


}
