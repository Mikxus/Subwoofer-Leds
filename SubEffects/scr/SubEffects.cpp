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
    pinMode(_subwooferPin,INPUT);
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
        Calculate();
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

void timer1::Start(uint16_t size, uint16_t freq)
{  
    uint16_t real_sample_rate;
    real_sample_rate = 16000000 / (freq * size) -1;
    cli();
                                            //set timer1 interrupt at 1kHz
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            //initialize counter value to 0 
                                            //OCR1A = real_sample_rate; // = (16*10^6) / (2000*64) - 1 (must be under 16bit) set compare match register for 1khz increments
    OCR1A = real_sample_rate;
    TCCR1B |= (1 << WGM12);                // turn on CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10);   // Set CS11 and CS10 bits for 64 prescaler
    TIMSK1 = (1 << OCIE1B);                // enable timer1 compare B interrupt
    sei();
}

void timer1::Stop()
{
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            //initialize counter value to 0
}

timer1::~timer1 ()
{
    Stop();
}

volatile uint16_t _fftArrPos;
ISR(TIMER1_COMPB_vect)
{
    if (!_fftBinReady)
    {
        uint16_t val = analogRead(_subwooferPin);
        if (_fftArrPos < sizeof(_vReal) )
        {
            // save the value to an array.
            _vReal[_fftArrPos] = analogRead(_subwooferPin);
            _fftArrPos += 1;
        } else
        {
            _fftBinReady = true;
        }
    }
}

