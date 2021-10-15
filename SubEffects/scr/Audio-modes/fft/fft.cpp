#include "fft.h"

bool fft::SetSampleSize(uint16_t size) {

    #ifdef DEBUG
        Serial.print(F("Updating FFT Sample Size to: "));
        Serial.println(_sampleSize);
    #endif // DEBUG

    _fftBinSize = size;
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

    _vReal = (double*) malloc(_fftBinSize * 4 + 1);
    _vImag = (double*) malloc(_fftBinSize * 4 + 1);
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
    if (!allocMem() ) 
    {
        #ifdef DEBUG
        Serial.println(F("FFT bin allocation failed"));
        Serial.print(F("Not enough memory for: "));
        Serial.print((uint32_t) _fftBinSize * 4 * 2);
        Serial.println(" Bytes");
        #endif
        return 0;

    }
    Start(_fftBinSize, _frequency );     // Calls timer1 wich setups the arduino uno's timer to interrupt at given frequency.
    return 1;
}

void fft::Stop() {

    Stop();
    deallocMem();
}

void fft::Calculate()
{
    if (_fftBinReady)
    {
        // if true do the fft calculations
        // not implemented yet
        _fftBinReady = false;              // after calculations set _arrReady to false
    }
}

fft::~fft()
{
    timer1::Stop();
}
