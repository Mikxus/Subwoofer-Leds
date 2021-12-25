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

        Start(_fftBinSize, _frequency );     // Calls timer1 wich setups the arduino uno's timer to interrupt at given frequency.
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
        _fftBinReady = false;             // after calculations set _arrReady to false
        return val;
    }
}
 
void fft::Benchmark()
{
    if (!_arrAllocated) 
    {
        if (!allocMem() )   // check if fft bin's allocation fails
        {
;
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
