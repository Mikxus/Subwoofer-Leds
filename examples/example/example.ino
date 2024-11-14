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


/* Create object of led_manager*/
led_manager effect_mgr = led_manager(); 

/* Create object of our custom light effect*/
colorBass bassEffect = colorBass();

/* Create new led strip struct */
struct ledStrip led_strip;

/* Led array */
CRGB leds[NUM_LEDS];

void setup()
{
  pinMode(AUDIO_PIN, INPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  Serial.begin(38400);
  delay(500);

  DEBUG(F("Example project"));

  /* Initialize FastLED with a new led strip */
  FastLED.addLeds<LED_CHIPSET, DATA_PIN, GRB>(&leds[0], NUM_LEDS); // GRB ordering is typical
  FastLED.setMaxRefreshRate(0);                                                    // Set refresh rate to unlimited for best performance                                                 //! Note may cause some led chipsets to flicker
  FastLED.setDither(0);

  /* Add led strip to effect manager */
  effect_mgr.add_led_strip(led_strip, &leds[0], NUM_LEDS);
  
  effect_mgr.add_effect(led_strip, bassEffect);

  bassEffect.set_color_palette(blueBass_p);
}

void loop()
{
  EVERY_N_MILLISECONDS(500)
  {
	INFO(F("FPS: "), FastLED.getFPS());
	FastLED.m_nFPS = 0;
  }

  PORTB |= B00010000; // pin 12 high

  effect_mgr.update();    // update the led strips

  PORTB &= B11101111; // pin 12 low
}