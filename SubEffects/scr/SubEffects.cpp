#include <Arduino.h>
//#include "SubEffects.h"
#include <FastLED.h>

void SubEffects::SubEffects(uint8_t subPin, uint8_t led_dataPin, uint16_t led_count)
    : _subwooferPin(subPin)
    , _ledDataPin(led_dataPin) 
    , _ledCount(led_count)
{
    pinMode(subPin,INPUT_PULLUP);
    pinMode(led_dataPin,OUTPUT);

}

void SubEffects::~SubEffects()
{
}

void Modes::Modes()
{
}

void Modes::Update() {
    switch (_currentMode)
    {
    case 0:
        if(!_initialized)
        {
            fft.Init();
        }
        fft.Calculate();
        break;
    
    default:
        break;
    }
}

void Modes::NextMode()
{
    if (_curentMode + 1 < _modeCount)
    {
        _currentMode += 1;
    }
}

void Modes::PreviousMode()
{
    if (_curentMode - 1 < _modeCount)
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

void timer1::Start()
{  
    uint16_t real_sample_rate;
    real_sample_rate = 16000000 / (_frequency * _samples_size) -1;
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

void fft::ISR(TIMER1_COMPB_vect)
{
    uint16_t val = analogRead(_subwooferPin);
    if (_arrPos < _samleSize)
    {
        // save the value to an array.
        *_vReal[_arrPos] = val;
        _arrPos += 1;
    } else
    {
        arrReady = true;
    }
}

void timer1::Stop()
{
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            //initialize counter value to 0
}


bool fft::SetSampleSize(uint16_t size) {

    _sampleSize = size;
    #ifdef DEBUG
        Serial.print(F("Updating FFT Sample Size to: "));
        Serial.println(_sampleSize);
    #endif // DEBUG
    Init();
}

void fft::SetFrequency(uint16_t freq)
{
    #ifdef DEBUG
        Serial.println(F("Updating timer1 Frequency to: "));
        Serial.println(_frequency);
    #endif // DEBUG
    _frequency = freq;
}

bool fft::Init();
{
    _vReal = (double*) malloc(size * 4 + 1);
    _vImag = (double*) malloc(size * 4 + 1);
    if (_vReal == nullptr || _vImag == nullptr)
    {
        #ifdef DEBUG
        Serial.println(F("Failed to allocate enough memory"));
        #endif // DEBUG

        return 0;                               // failed to allocate enough memory
    }
    timer1.Start();
}

void fft::Stop() {

    timer1.Stop();
    if (_arrAllocated)
    {
        free(_vReal);
        free(_vImag);
        _arrAllocated = 1;
        *_vReal = nullptr;
        *_vImag = nullptr;
    }
}

void fft::Calculate()
{
    if (_arrReady)
    {
        // array ready do the calculations
        _arrReady = false;              // after calculations set _arrReady to false
    }
}

void fft::~fft()
{
    Stop();
}
