#ifndef LEDCONTROL_H
#include <ledControl.h>
#endif

void ledControl::Init(uint8_t led_dataPin, uint16_t led_count, CFastLED& fastLedObj, CRGB leds[])
{
    _FastLED = &fastLedObj;
    _led_count = led_count;
    _leds = leds;               // Save the pointer
}

void ledControl::Fade(uint8_t hue,uint16_t brightness)   
{
    //bright._Alpha = 0.07;                                 // Temporary values used for debug
    color._r = 1.0;
   if (hue == 0) hue = _lasthue;                   // if the color value is 0 use the last value;
    else _lasthue = hue;

    float val =  bright.calc(brightness);
    if (!val) val = 0;
    //Serial.print(F("b: ")); Serial.print( val,2); Serial.print(F("_r: ")); Serial.print(bright._r,2); Serial.print(F(" _Alpha: ")); Serial.println(bright._Alpha,2);// Serial.print(F(" _deltaTime: ")); Serial.println(_deltaTime);
    float colorVal = color.calc(hue);

    val = map(val, 0, 500, 0, 255);                         
    for(uint16_t i = 0; i < _led_count; i++)                // Calculate each leds value
    {
        _leds[i].setHSV(colorVal, 255, val);
    }
    _FastLED->show();                                       // Update the leds

    _deltaTime = micros() - _lastUpdate;
    _lastUpdate = micros();

    _lastBrightness = brightness;
}

ledControl::~ledControl()
{
}