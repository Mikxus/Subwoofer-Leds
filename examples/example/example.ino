#include <Arduino.h>
#include "D:\Code\Github\Subwoofer-Leds\scr\SubEffects.h"
#include <FastLED.h>

#define NUM_LEDS 58     // Define the number of leds
#define DATA_PIN 6      // Define the data pin for leds

CRGB leds[NUM_LEDS];    // Initialize the array
CFastLED FastObj;       // Create CFastLED object, then pass to Subeffect's library.

SubEffects effect( 0, DATA_PIN, NUM_LEDS, FastObj, leds);         // effect( audio pin , Led's data pin, Led count, FastLED object, Led array)

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(38400);
  effect.CalibrateSoundLevel();                             // 1,5s delay + calibrates the noise level.
  //Serial.print(F("New zero is:"));
  //Serial.println(_calibratedNoiseZero);
  delay(1000);
  FastObj.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setMaxRefreshRate(0);                             // For best performance. !May cause some led strips to flicker                                                           
}

void loop() {
  float updates;
  unsigned long last_fps = 0;
    while (true)
    {
      if (millis() > last_fps + 250)
      {
        //updates * 1000.0 / 250.0;
        Serial.print("FPS: ");
        Serial.println(FastObj.getFPS());
        //updates = 0;
        last_fps = millis();
      }
      effect.Update();
    }
}
