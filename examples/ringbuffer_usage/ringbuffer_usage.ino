#include <FastLED.h>
#include <SubEffects.h>

/* remember to change this to your absolute path
 * since arduino doesn't support relative paths :)
 */
#include "/home/mikko/snap/arduino-cli/current/Arduino/libraries/SubEffects/src/utils/data_types/ringbuffer.h"

#define NUM_LEDS 58
#define DATA_PIN 6
#define LED_CHIPSET WS2812B
#define COLOR_ORDER GRB
#define AUDIO_INPUT_PIN 0

led_manager effect_mgr;

ledStrip led_strip;

CRGB leds[NUM_LEDS];

class test_ringbuffer : public audioMode
{
private:
    uint8_t test_state = 0;
    uint8_t test_counter = 0;

    void test_ringbuffer_push()
    {
        CRGB color;

        if (test_counter == 255)
        {
            INFO(F("Test rinbuffer_push() done"));
            test_counter = 0;
            test_state++;
            return;
        }

        color = CHSV(random(), 255, 255);
        r_buffer.push(color);
        test_counter++;
    }

    void test_ringbuffer_pop()
    {
        if (test_counter == 255)
        {
            INFO(F("Test rinbuffer_pop() done"));
            test_counter = 0;
            test_state++;
            return;
        }
        r_buffer.pop();
        test_counter++;
    }

    virtual void on_resize() override
    {
        r_buffer.resize(&led_array[0], led_array.size());
    }

public:
    ringbuffer<CRGB> r_buffer = ringbuffer<CRGB>(&led_array[0], led_array.size());

    test_ringbuffer() = default;
    ~test_ringbuffer() = default;

    virtual bool update()
    {
        switch (test_state)
        {
        case 0:
            test_ringbuffer_push();
            delay(5);
            break;

        case 1:
            test_ringbuffer_pop();
            delay(5);
            break;

        case 2:
            test_ringbuffer_push();
            delay(5);
            break;


        default:
            delay(5000);
            INFO(F("Tests done"));
            break;
        }

        return 1;
    }
};

test_ringbuffer test_ringbuffer_obj;
test_ringbuffer test_ringbuffer_obj2;

void setup()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(AUDIO_INPUT_PIN, INPUT);

    Serial.begin(38400);
    delay(500);
    DEBUG(F("Ringbuffer usage test sketch"));

    FastLED.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(&leds[0], NUM_LEDS); // GRB ordering is typical

    /*
     * Conflicts with subeffect's way of updating the effects
     * since not all effects may be updated on a FastLED.show()
     */
    FastLED.setMaxRefreshRate(0);
    FastLED.setDither(0);

    effect_mgr.add_led_strip(led_strip, &leds[0], NUM_LEDS);

    /* Set the ringbuffer test effect to whole led strip */
    effect_mgr.add_effect(led_strip, test_ringbuffer_obj, &leds[0], &leds[20]);
    effect_mgr.add_effect(led_strip, test_ringbuffer_obj2, &leds[21], &leds[NUM_LEDS]);

    FastLED.setBrightness(10);
}

void loop()
{
    effect_mgr.update();
}