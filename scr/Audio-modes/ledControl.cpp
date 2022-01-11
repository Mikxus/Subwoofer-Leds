#ifndef LEDCONTROL_H
#include <ledControl.h>
#endif


void ledControl::Init(uint8_t led_dataPin, uint16_t led_count, CFastLED& fastLedObj, CRGB leds[])
{
    _FastLED = &fastLedObj;
    _led_count = led_count;
    _leds = leds;               // Save the pointer              // I'm quite sure i'll get memory leaks of my great code
}

//uint16_t fps;
//unsigned long fpsUp = 0;
void ledControl::Fade(uint8_t bassHz,uint16_t brightness)   
{
    //bright._Alpha = 0.07;                                       // Temporary values
    color._r = 1.0;

   if (bassHz == 0) bassHz = _lastBassHz;                   // if the color value is 0 use the last value;
    else _lastBassHz = bassHz;

    float val =  bright.calc(brightness);
    if (!val) val = 0;
    //Serial.print(F("b: ")); Serial.print( val,2); Serial.print(F("_r: ")); Serial.print(bright._r,2); Serial.print(F(" _Alpha: ")); Serial.println(bright._Alpha,2);// Serial.print(F(" _deltaTime: ")); Serial.println(_deltaTime);
    float colorVal = color.calc(bassHz);

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