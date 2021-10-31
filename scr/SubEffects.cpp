#include <Arduino.h>
#include <FastLED.h>
#include "SubEffects.h"

uint8_t _subwooferPin;      // global variable for subwoofer pin
                            // used in timer1's ISR

SubEffects::SubEffects(uint8_t subPin, uint8_t led_dataPin, uint16_t led_count)
    : _ledDataPin(led_dataPin) 
    , _ledCount(led_count)
{
    _subwooferPin = subPin;
    pinMode(_subwooferPin,INPUT_PULLUP);
    pinMode(led_dataPin,OUTPUT);
}

SubEffects::~SubEffects()
{
}

Modes::Modes()
{
}

void Modes::Update() {
    switch (_currentMode)
    {
    case 0:
        if(!_initialized)
        {
            if( Init() )
            {
                #ifdef DEBUG
                Serial.println(F("FFT initialized succesfully"));
                #endif
                _initialized = true;
            } else 
            {
                 #ifdef DEBUG
                Serial.println(F("FFT failed to initialized"));
                #endif
                break;
                _initialized = false;
            }
        }
        uint16_t val = Calculate();
        val = map(val,0,255,0,359);
        LinearFade(val);
        break;
    
    default:
        break;
    }
}

void Modes::NextMode()
{
    if (_currentMode + 1 < _modeCount)
    {
        _currentMode += 1;
    }
}

void Modes::PreviousMode()
{
    if (_currentMode - 1 < _modeCount)
    {
        _currentMode -= 1;
    }
}

void Modes::SetMode(uint8_t mode)
{
    if (mode < _modeCount)
    {
        _currentMode = mode;
    }
}

volatile uint16_t _fftArrPos;
ISR(TIMER1_COMPB_vect)
{
    if (!_fftBinReady)
    {
        uint16_t val = analogRead(_subwooferPin);
        if (_fftArrPos < _fftBinSize )
        {
            // save the value to an array.
            _vReal[_fftArrPos] = val;
            _fftArrPos += 1;
        } else
        {
            _fftArrPos = 0;
            _fftBinReady = true;
        }
    }
}