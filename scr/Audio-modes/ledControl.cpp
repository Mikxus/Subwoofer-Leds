#ifndef LEDCONTROL_H
#include <ledControl.h>
#endif

void ledControl::Init(uint8_t led_dataPin, uint16_t led_count, CFastLED& fastLedObj, CRGB leds[])
{
    _FastLED = &fastLedObj;
    _led_count = led_count;
    _leds = leds;               // Save the pointer
}

void ledControl::logTime()
{
    _deltaTime = micros() - _lastUpdate;
    _lastUpdate = micros();
}

void ledcontrol::logLastHSV(uint8_t hue, uint8_t saturation, uint8_t value)
{
    _lastHue = hue;
    _lastSaturation = saturation;
    _lastValue = value;
}

void ledControl::Fade(uint8_t hue,uint16_t brightness)   
{
    //bright._Alpha = 0.07;                                 // Temporary values used for debug
    color._r = 0.0005;
   if (hue == 0) hue = _lastHue;                            // if the color value is 0 use the last value;
    else _lastHue = hue;

    float val =  bright.calc(brightness);
    float colorVal = color.calc(hue);
    //Serial.print("val "); Serial.println(colorVal);
    //Serial.flush();
    val = map(val, 0, 500, 0, 255);
    if (val < 1) val = 0;  
    
    _leds[0].setHSV(colorVal, 255, val);
    _FastLED->showColor(_leds[0],brightness);               // not sure if works. But should set all leds to single color in _leds[0]
    /*                 
    for(uint16_t i = 0; i < _led_count; i++)                // Calculate each leds value
    {
        _leds[i].setHSV(colorVal, 255, val);
    }
    */
    _FastLED->show();                                       // Update the leds

    logTime();                                              // log the time used.

    logLastHSV(hue, 255, brightness);                       // save the last hsv values
}

void ledControl::BreathingStatic(uint8_t hue, uint8_t saturation,uint8_t fromLow, uint8_t toHigh)
{
    uint8_t value;
    weighted_moving_average_filter breath;
    breath._r = 0.001;
    

    logLastHSV( hue , saturation , value );
    logTime();
}

ledControl::~ledControl()
{
}