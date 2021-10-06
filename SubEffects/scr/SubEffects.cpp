#include <Arduino.h>
#include <SubEffects.h>

bool SubEffects::SubEffects(uint8_t pin = 0,uint8_t led_pin,uint16_t led_amount)
    : _frequency(freq)
    , _subwoofer_pin(pin) 
    , _sample_size(size)
{
    modes();
}

bool SubEffects::update()
{
    modes();
}

SubFFT::~SubEffects()
{
    free(_vReal);
    free(_vImag);
}

void modes::Modes()
{
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

void timer1::Stop()
{
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            //initialize counter value to 0
}


bool fft::init();
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
    timer1.start();
}