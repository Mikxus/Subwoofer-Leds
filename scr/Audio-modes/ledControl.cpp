

#ifndef LEDCONTROL_H
#include <ledControl.h>
#endif

#ifndef SUBEFFECTS_H
#error "SubEffects.h is not included"
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
    bright._Alpha = 0.07;
    color._Alpha = 0.04;
   if (bassHz == 0) bassHz = _lastBassHz;                   // if the color value is 0 use the last value;
    else _lastBassHz = bassHz;

    float val =  bright.calc(brightness);
    float colorVal = color.calc(bassHz);
    /*Serial.print("A: ");
    Serial.print(colorVal,2);
    Serial.print(" X: ");
    */
    val = map(val, 0, 500, 0, 255);
    /*Serial.println(val,2);
    Serial.flush();*/
    for(uint16_t i = 0; i < _led_count; i++)
    {
        _leds[i].setHSV(colorVal, 255, val);
    }
    _FastLED->show();

    _realUpdateRate = 1000 / ( millis() - _lastUpdate );
    _lastUpdate = millis();

    _lastBrightness = brightness;
    //fps += 1;
}

ledControl::~ledControl()
{
    //delete leds;
}