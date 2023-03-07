#include <Arduino.h>
#include <FastLED.h>
#include <inttypes.h>
#include <SubEffects.h>

/* Definitions for the led strip */
#define NUM_LEDS 58              // number of leds
#define DATA_PIN 6               // data pin for leds
#define AUDIO_PIN 0              // analog pin for the led strip
#define LED_CHIPSET WS2812B      // Led chipset
#define COLOR_ORDER GRB          // led strip's color order: rgb grb brg
/* ----------------------------- */


CFastLED FastObj;                 // Create CFastLED object.

SubEffects effect( &FastObj );    // Create SubEffects object then pass CFastLED to it


void setup() {
  pinMode(AUDIO_PIN, INPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  Serial.begin(38400);
  delay(500);
  DEBUG(F("Example project"));
  
  // Add new ledstrip for the SubEffect object
  uint8_t ledID = 0;
  ledID = effect.AddLedStrip( AUDIO_PIN, 0, NUM_LEDS);

  // Add new ledstrip for the FastLED object
  FastObj.addLeds<LED_CHIPSET, DATA_PIN, GRB>( effect.GetLedsPtr( ledID ) , NUM_LEDS);  // GRB ordering is typical
  FastObj.setMaxRefreshRate(0);                                                         // Set refresh rate to unlimited for best performance                                                 //! Note may cause some led chipsets to flicker  
  FastObj.setDither(0);

  // Calibrate all led strips' input values
  effect.CalibrateNoise(); 
}

void loop() {
  effect.Update();              // Update the led strips
}