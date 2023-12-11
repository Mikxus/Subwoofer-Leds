#include <inttypes.h>
#include <FastLED.h>
#include <SubEffects.h>

/* Definitions for the led strip */
#define NUM_LEDS 58         // number of leds
#define DATA_PIN 6          // data pin for leds
#define AUDIO_PIN 0         // analog pin for the led strip
#define LED_CHIPSET WS2812B // Led chipset
#define COLOR_ORDER GRB     // led strip's color order: rgb grb brg
/* ----------------------------- */

led_manager effect_mgr; // Create SubEffects object then pass CFastLED to it

ledStrip led_strip; // led strip object

CRGB leds[NUM_LEDS]; // led array for the led strip

/**
 * @brief example of a simple audio mode.
 *
 */
class simple_rainbow : public audioMode
{
  uint8_t hue = 0;
  uint8_t speed = 80;

public:
  simple_rainbow() = default;
  ~simple_rainbow() = default;

    virtual bool update()
    {
        fill_rainbow(&led_array[0], led_array.size(), beat8(speed, 255), 10 );
        return 1;
    }
};

simple_rainbow rainbow_obj;

void setup()
{
  pinMode(AUDIO_PIN, INPUT);
  pinMode(DATA_PIN, OUTPUT);

  Serial.begin(38400);
  delay(500);
  DEBUG(F("Example audio mode"));


  // Add new ledstrip for the FastLED object
  FastLED.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(&leds[0], NUM_LEDS); // GRB ordering is typical
  FastLED.setMaxRefreshRate(0);                                                            // Set refresh rate to unlimited for best performance
  FastLED.setDither(0);

  effect_mgr.add_led_strip(led_strip, &leds[0], NUM_LEDS);

  /* Now load our own led effect */
  effect_mgr.add_effect(led_strip, rainbow_obj);
}

/* Updates the ledstrip */
void loop()
{
  effect_mgr.update();
}