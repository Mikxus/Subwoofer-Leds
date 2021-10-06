#include <Arduino.h>
#include <SubEffects.h>
#include <FastLED.H>

void setup() {
SubEffects effect(0,6,100);

void setup() {
    Serial.begin(115200);
}

void loop() {
    while (true)
    {
    effect.update();
    }
}
