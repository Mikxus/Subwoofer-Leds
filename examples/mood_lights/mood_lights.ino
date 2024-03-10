#include <FastLED.h>
#include <SubEffects.h>

#define NUM_LEDS 58
#define DATA_PIN 6
#define LED_CHIPSET WS2812B
#define COLOR_ORDER GRB
#define AUDIO_INPUT_PIN 0

led_manager effect_mgr;

ledStrip led_strip;

CRGB leds[NUM_LEDS];

class mood_lights : public audioMode
{
public:
    mood_lights() = default;
    ~mood_lights() = default;

    uint8_t fade_speed = 3;
    uint8_t fade_progress = 0;

    CHSV target_color = CHSV(random8(), 255, 0);
    CHSV current_color;

    virtual bool update()
    {
        if (fade_progress == 255)
        {
            target_color = CHSV(random8(), 255, 255);
            fade_progress = 0;
            DEBUG(F("new hue: "), target_color.hue);
        }

        EVERY_N_MILLIS(1000 / 60)
        {
            fade_progress = qadd8(fade_progress, fade_speed);
            current_color = blend(current_color, target_color, fade_progress); 
            fill_solid(&led_array[0], led_array.size(), current_color);
            return 1;
        }

        return 0;
    }

};

mood_lights mood_lights_obj;

void setup()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(AUDIO_INPUT_PIN, INPUT);

    Serial.begin(38400);
    delay(500);
    DEBUG(F("Mood lights example"));

    FastLED.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(&leds[0], NUM_LEDS); // GRB ordering is typical
    
    /* 
     * Conflicts with subeffect's way of updating the effects
     * since not all effects may be updated on a FastLED.show()
     */
    FastLED.setMaxRefreshRate(0);
    FastLED.setDither(0);

    effect_mgr.add_led_strip(led_strip, &leds[0], NUM_LEDS);
    effect_mgr.add_effect(led_strip, mood_lights_obj);

    FastLED.setBrightness(10);
}

void loop()
{
    effect_mgr.update();
}