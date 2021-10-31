#include <Arduino.h>
//#include "D:\Code\Github\Subwoofer-Leds\SubEffects\scr\SubEffects.h"
#include <FastLED.h>

#define NUM_LEDS 54     // Define the number of leds
#define DATA_PIN 6      // Define the data pin for leds
FastLED

CRGB leds[NUM_LEDS];
SubEffects effect(0,DATA_PIN,NUM_LEDS);         // effect( subwoofer analog pin , led data pin , led count )

void setup() {
    Serial.begin(115200);
    delay(1500);

    effect.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void loop() {
    while (true)
    {
    effect.Update();
    }
}
