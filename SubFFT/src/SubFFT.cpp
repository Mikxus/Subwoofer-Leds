#include SubFFT.h

bool SubFFT::Begin(uint8_t pin,uint16_t freq, uint16_t size)
    : _frequency(freq)
    , _subwoofer_pin(pin) 
    , _sample_size(size)
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
    timer1()
}

SubFFT::~SubFFT()
{
    free(_vReal);
    free(_vImag);
}