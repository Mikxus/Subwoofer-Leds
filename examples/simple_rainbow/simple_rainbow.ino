#include <inttypes.h>
#include <FastLED.h>
#include <SubEffects.h>

/* Definitions for the led strip */
#define NUM_LEDS 58         // number of leds
#define DATA_PIN 6          // data pin for leds
#define LED_CHIPSET WS2812B // Led chipset
#define COLOR_ORDER GRB     // led strip's color order: rgb grb brg

/**
 * Led manager object 
 *  Represents a all the led strips and their effects
 */
led_manager effect_mgr;

/**
 *  Led strip struct
 *  the ledStrip struct represents a single physical led strip
 */
ledStrip led_strip; 

/* Led array for the led strip */
CRGB leds[NUM_LEDS];

/**
 * @brief example of a simple audio mode.
 */
class simple_rainbow : public audioMode
{
public:
  uint8_t hue = 0;
  uint8_t speed = 80;

  simple_rainbow() = default;
  ~simple_rainbow() = default;

    /* update function which draws a single "frame of the pixels"  
     * Note: It uses led_array to access the CRGB pixels 
     * to enable changing of the effect's pixel output range easily
     */
    virtual bool update()
    {
        fill_rainbow(&led_array[0], led_array.size(), beat8(speed, 255), 10 );
        return 1;
    }
};

simple_rainbow rainbow_obj;
simple_rainbow rainbow_obj_fast;

void setup()
{
  pinMode(DATA_PIN, OUTPUT);

  Serial.begin(38400);
  delay(500);
  DEBUG(F("Example audio mode"));


  // Add new ledstrip for the FastLED object
  FastLED.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(&leds[0], NUM_LEDS); // GRB ordering is typical
  FastLED.setMaxRefreshRate(0);                                                            // Set refresh rate to unlimited for best performance
  FastLED.setDither(0);

  effect_mgr.add_led_strip(led_strip, &leds[0], NUM_LEDS);

  /* 
   * Now load our own led effects 
   * And set the effects output range the whole strip
   */
  effect_mgr.add_effect(led_strip, rainbow_obj, &leds[0], &leds[29]);

  /* set rainbow_obj_fast's animation speed really fast */
  rainbow_obj_fast.speed = 200;
  effect_mgr.add_effect(led_strip, rainbow_obj_fast, &leds[29], &leds[58]);
}

/* Updates the ledstrip */
void loop()
{
  effect_mgr.update();
}