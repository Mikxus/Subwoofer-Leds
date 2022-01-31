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
    if ( size < freeMemory()) 
    {
        _fftBinSize = size;
        return 1;
    } else {
        return 0;
    }
    
}

void fft::SetFrequency(uint16_t freq)
{
    #ifdef DEBUG
        Serial.println(F("Updating timer1 Frequency to: "));
        Serial.println(_frequency);
        Serial.flush();
    #endif // DEBUG
    _frequency = freq;
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
        _vReal = NULL;
        _vImag = NULL;

        #ifdef DEBUG
        Serial.println(F("Deallocated the fft bins"));
        Serial.println(F("Free mem: ")); 
        Serial.println( freeMemory() );
        Serial.flush();
        #endif // DEBUG
    } else return;
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
        Serial.flush();
        #endif
        return 0;

    } else
    {
        #ifdef DEBUG
        Serial.println(F("FFT bin alloction succesfull"));
        Serial.print(F("Allocated: "));
        Serial.print(_fftBinSize * sizeof(double));
        Serial.println(" Bytes for fft bins");
        Serial.flush();
        #endif
        uint16_t achievedFreq;
        achievedFreq = Start(_frequency );     // Calls timer1 wich setups the arduino uno's timer to interrupt at given frequency.
        /*Serial.print(F("Achieved freq: "));
        Serial.print(achievedFreq);
        Serial.print(F(" Wanted freq: "));
        Serial.println(_frequency);
        delay(1500);*/
    }
    return 1;
}

void fft::Stop() {

    timer1::Stop();
    deallocMem();
}

uint16_t fft::Calculate()
{
    if (_fftBinReady)
    {   
        cli();
        FFT.Windowing(_vReal, _fftBinSize, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.Compute(_vReal, _vImag, _fftBinSize, FFT_FORWARD);
        FFT.ComplexToMagnitude(_vReal, _vImag, _fftBinSize);
        uint16_t val = FFT.MajorPeak(_vReal, _fftBinSize, _frequency);   // find the peak hz
        sei();
        /*                                          // for debug 
        Serial.println(F("DEBUG"));
        Serial.print(F("Printing _vReal array: "));
        Serial.print(sizeof(double) * _fftBinSize);
        Serial.println(" Bytes");
        cli();
                                             
        for (int i = 0; i < _fftBinSize; i++)
        {
            if (i % 10 == 0) Serial.println();
            
            Serial.print((uint32_t) _vReal[i]);
            if (i % 10 != 9) Serial.print(F(","));
        }
        Serial.println();
        sei();
        */ 
        for (uint16_t i = 0; i < _fftBinSize; i++)
        {
            _vImag[i] = 0;
        }
        _fftBinReady = false;             // after calculations set _fftBinReady to false
        return val;
    } else return 0;
}

uint16_t fft::CalculateOptimized()
{
    if (_fftBinReady)
    {
        
    }
}
 
void fft::Benchmark()
{
    if (!_arrAllocated) 
    {
        if (!allocMem() )   // check if fft bin's allocation fails
        {
            return;          // if it does, No need to calculate fft when there is no data.
        }

    }
    _fftBinReady = 1;
    Calculate();
}

fft::~fft()
{
    Stop();
}
