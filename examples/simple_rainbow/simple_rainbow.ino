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

CFastLED FastObj; // Create CFastLED object.

SubEffects effect(&FastObj); // Create SubEffects object then pass CFastLED to it

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

    virtual bool Update()
    {
        fill_rainbow(_ledStrip->ledArr, _ledStrip->led_rgb_data_size, beat8(speed, 255), 10 );
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

  // Add new ledstrip for the SubEffect object
  uint8_t ledID = 0;
  ledID = effect.AddLedStrip(AUDIO_PIN, 0, NUM_LEDS);

  // Add new ledstrip for the FastLED object
  FastObj.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(effect.GetLedsPtr(ledID), NUM_LEDS); // GRB ordering is typical
  FastObj.setMaxRefreshRate(0);                                                            // Set refresh rate to unlimited for best performance
  FastObj.setDither(0);

  /* Calibrate all led strips' input values */
  effect.CalibrateNoise();

  /* Now load our own led effect */
  effect.loadMode(&rainbow_obj, ledID);
}

/* Updates the ledstrip */
void loop()
{
  effect.Update();
}