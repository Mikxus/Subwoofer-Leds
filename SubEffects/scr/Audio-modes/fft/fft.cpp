#include "fft.h"
bool fft::SetSampleSize(uint16_t size) {

    #ifdef DEBUG
        Serial.print(F("Updating FFT Sample Size to: "));
        Serial.println(_sampleSize);
    #endif // DEBUG

    _sampleSize = size;
    if ( Init() ) 
    {
        return 1;
    } else return 0;
}

void fft::SetFrequency(uint16_t freq)
{
    #ifdef DEBUG
        Serial.println(F("Updating timer1 Frequency to: "));
        Serial.println(_frequency);
    #endif // DEBUG
    _frequency = freq;
}

bool fft::allocMem()
{
    if (_arrAllocated) deallocMem();

    _vReal = (double*) malloc(_sampleSize * 4 + 1);
    _vImag = (double*) malloc(_sampleSize * 4 + 1);
    if (_vReal == NULL || _vImag == NULL)
    {
        #ifdef DEBUG
        Serial.println(F("Failed to allocate enough memory"));
        #endif // DEBUG

        return 0;                               // failed to allocate enough memory
    }
    _arrAllocated = 1;
    return 1;
}

void fft::deallocMem()
{       
        free(_vReal);
        free(_vImag);
        _arrAllocated = 0;
        _vReal = NULL;
        _vImag = NULL;

        #ifdef DEBUG
        Serial.println(F("Deallocated the fft bins"));
        #endif // DEBUG
}

bool fft::Init()
{
    allocMem();
    Start(_sampleSize, _frequency )     // Calls timer1 wich setups the arduino uno's timer to interrupt at given frequency.
    return 1;
}

void fft::Stop() {

    timer1.Stop();
    deallocMem();
}

void fft::Calculate()
{
    if (_arrReady)
    {
        // if true do the fft calculations
        // not implemented yet
        _arrReady = false;              // after calculations set _arrReady to false
    }
}

fft::~fft()
{
    Stop();
}
