#include <Arduino.h>
#include "/home/mikko/Documents/code/Subwoofer-Leds/scr/SubEffects.h"
#include <FastLED.h>

#define NUM_LEDS 54     // Define the number of leds
#define DATA_PIN 6      // Define the data pin for leds
CRGB leds[NUM_LEDS];

SubEffects effect(0,NUM_LEDS,DATA_PIN);         // effect( subwoofer analog pin , led count, FastLed object )

void setup() {
    Serial.begin(115200);
    delay(1500);

    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void loop() {
    while (true)
    {
    effect.Update();
    }
}
