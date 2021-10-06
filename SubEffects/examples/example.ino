#include <Arduino.h>
#include <SubEffects.h>
#include <FastLED.h>

void setup() {
SubEffects effect(0,6,100);         // effect( subwoofer analog pin , led data pin , led count )

void setup() {
    Serial.begin(115200);
}

void loop() {
    while (true)
    {
    effect.update();
    }
}
