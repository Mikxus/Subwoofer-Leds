/*
 * MIT License
 * 
 * Copyright (c) 2021 Mikko Johannes Heinänen 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

//#define DEBUG

bool fft::SetSampleSize(uint16_t size) {
    #ifdef DEBUG
        Serial.print(F("Updating FFT Sample Size to: "));
        Serial.print(size);
        Serial.println(F("Bytes"));
        Serial.flush();
    #endif // DEBUG

    deallocMem();
    if ( size * sizeof(double) * 2 < freeMemory() ) 
    {
        _fftBinSize = size;
        return 1;
    }
    return 0;   
}

void fft::SetFftFrequency(uint32_t freq)
{
    #ifdef DEBUG
        Serial.println(F("Updating timer1 Frequency to: "));
        Serial.println(_frequency);
        Serial.flush();
    #endif // DEBUG
    _frequency = SetTimerFrequency(freq);
    return;
}

bool fft::allocMem()
{
    if (_arrAllocated) deallocMem();

    _vReal =  (double*) malloc(sizeof(double) * _fftBinSize );
    _vImag =  (double*) calloc( _fftBinSize,sizeof(double) );
    if (_vReal == NULL || _vImag == NULL)
    {
        #ifdef DEBUG
        Serial.println(F("Failed to allocate enough memory"));
        Serial.print(F("Free mem: ")); 
        Serial.println( freeMemory() );
        Serial.print(F("Required mem: "));
        Serial.println(sizeof(double) * _fftBinSize);
        Serial.flush();
        #endif // DEBUG

        if (_vReal != NULL) free(_vReal);       // check if variable got allocated, then free it
        if (_vImag != NULL) free(_vImag);       // same here

        return 0;                               // failed to allocate enough memory
    }
    _arrAllocated = 1;
    return 1;
}

void fft::deallocMem()
{      
    if (_arrAllocated)
    {
        free(_vReal);
        free(_vImag);
        _arrAllocated = 0;
        _vReal = nullptr;
        _vImag = nullptr;

        #ifdef DEBUG
            Serial.println(F("Deallocated the fft bins"));
            Serial.println(F("Free mem: ")); 
            Serial.println( freeMemory() );
            Serial.flush();
        #endif // DEBUG
    }
    return;
}

bool fft::Init()
{
    if (!allocMem() ) 
    {
        #ifdef DEBUG
            Serial.println(F("FFT bin allocation failed"));
            Serial.print(F("Not enough memory for: "));
            Serial.print(_fftBinSize * sizeof(double));
            Serial.println(F(" Bytes"));
            Serial.flush();
            #endif
        return 0;

    } else
    {
        #ifdef DEBUG
            Serial.println(F("FFT bin alloction succesfull"));
            Serial.print(F("Allocated: "));
            Serial.print(_fftBinSize * sizeof(double));
            Serial.println(F(" Bytes for fft bins"));
            Serial.flush();
        #endif
        _frequency = Start( _frequency );     // Calls timer1 wich setups the arduino uno's timer to interrupt at given frequency.
    }
    return 1;
}

void fft::Stop() {

    timer1::Stop();
    deallocMem();
    return;
}

uint16_t fft::Calculate()
{
    if (_fftBinReady)
    {   
        FFT.Windowing(_vReal, _fftBinSize, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // takes approx 5ms
        FFT.Compute(_vReal, _vImag, _fftBinSize, FFT_FORWARD);                // takes approx 16ms
        FFT.ComplexToMagnitude(_vReal, _vImag, _fftBinSize);                  // takes approx < 2ms
        uint16_t val = FFT.MajorPeak(_vReal, _fftBinSize, _frequency);        // find the peak hz. Takes approx < 1ms;

        for (uint16_t i = 0; i < _fftBinSize; i++)
        {
            _vImag[i] = 0;
        }
        _fftBinReady = false;             // after calculations set _fftBinReady to false
        return val;
    }
    return 0;
}

uint16_t fft::CalculateOptimized()
{
    if (_fftBinReady)
    {
        
    }
}

fft::~fft()
{
    Stop();
}
