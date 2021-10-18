#include "fft.h"

#define DEBUG

bool fft::SetSampleSize(uint16_t size) {

    #ifdef DEBUG
        Serial.print(F("Updating FFT Sample Size to: "));
        Serial.print(_fftBinSize * 2);
        Serial.println(F("Bytes"));
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

    _vReal =  new double [ sizeof(double) * _fftBinSize ];
    _vImag =  new double [ sizeof(double) * _fftBinSize ];
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
        Serial.print(_fftBinSize * sizeof(double));
        Serial.println(" Bytes");
        #endif
        return 0;

    } else
    {
        #ifdef DEBUG
        Serial.println(F("FFT bin alloction succesfull"));
        Serial.print(F("Allocated: "));
        Serial.print(_fftBinSize * sizeof(double));
        Serial.println(" Bytes for fft bins");
        #endif

        Start(_fftBinSize, _frequency );     // Calls timer1 wich setups the arduino uno's timer to interrupt at given frequency.
    }
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

        Serial.println(F("DEBUG"));
        Serial.print(F("Printing _vReal array: "));
        Serial.print(sizeof(double) * _fftBinSize);
        Serial.println(" Bytes");

        cli();
        for (int i = 0; i < _fftBinSize; i++)
        {
            if (i % 10 == 0) Serial.println();
            
            Serial.print(_vImag[i]);
        }
        Serial.println();
        sei();
        _fftBinReady = false;             // after calculations set _arrReady to false
    }
}

fft::~fft()
{
    timer1::Stop();
}
