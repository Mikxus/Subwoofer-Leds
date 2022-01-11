//#include <Arduino.h>

//#DEBUG

SubEffects::SubEffects(uint8_t subPin, uint8_t led_dataPin,uint16_t led_amount, CFastLED & fastLedObj, CRGB ledObj[])
{
    _subwooferPin = subPin;
    pinMode(_subwooferPin,INPUT_PULLUP);
    pinMode(led_dataPin,OUTPUT);
    ledController.Init(led_dataPin,led_amount, fastLedObj, ledObj);
}

void SubEffects::CalibrateSoundLevel()
{
    uint16_t CalibrateTime = millis();
    _calibratedNoiseZero = 0;

    while ( millis() < CalibrateTime + 1500 )
    {
        uint16_t val = analogRead(_subwooferPin);
        if (val > _calibratedNoiseZero)
        {
            _calibratedNoiseZero = val;
        }
    }   
    _calibratedNoiseZero += 5;
    constrain(_calibratedNoiseZero, 0, 1023);
}

SubEffects::~SubEffects()
{
}

Modes::Modes()
{
}

void Modes::Update() {          // This chooses wich "mode" to use.
    switch (_currentMode)
    {
    case 0:
        if(!_initialized)       // Checks if all necessary components are initialized
        {
            if( fft::Init() )
            {
                #ifdef DEBUG
                Serial.println(F("FFT initialized succesfully"));
                #endif
                _initialized = true;
            } else 
            {
                 #ifdef DEBUG
                Serial.println(F("FFT failed to be initialized"));
                #endif
                break;
                _initialized = false;
            }
        }
        uint16_t val;
        val = fft::Calculate();                             // Calculates the strongest frequency.
        val = constrain(val,0,255);
        uint16_t brightness = analogRead(_subwooferPin);
        brightness = constrain(brightness, _calibratedNoiseZero, 450);
        brightness = map(brightness,_calibratedNoiseZero,450,0,1023);
        ledController.Fade(val, brightness); // Calls the ledcontrollers basic fade function to update the leds.
        break;
    
    default:
        break;
    }
}

void Modes::NextMode()
{
    if (_currentMode + 1 < _modeCount) _currentMode++;
    else _currentMode = 0;
}

void Modes::PreviousMode()
{
    if (_currentMode != 1) _currentMode--;              // if variable mode - 1 is 0 set the variable to the max amount of "modes"
    else _currentMode = _modeCount;
}

void Modes::SetMode(uint8_t wantedMode)
{
    if (wantedMode <= _modeCount)
    {
        _currentMode = wantedMode;
    }
}

volatile uint16_t _fftArrPos;
ISR(TIMER1_COMPB_vect)                              // Interrupt routine for the fft library. each iteration it checks if the sapling has completed. If not it saves the current analog input to the array
{
    if (!_fftBinReady)
    {
        uint16_t val = analogRead(_subwooferPin);
        if ( val <= _calibratedNoiseZero )           // check if the reading is above the noise level if it isn't sets the value to zero
        {
            val = 0;
        }
        if (_fftArrPos < _fftBinSize )              // if the _fftArrPos is within the array size save the value. else set the value to zero and set _fftBinReady to true
        {
            _vReal[_fftArrPos] = val;
            _fftArrPos += 1;
        } else
        {
            _fftArrPos = 0;
            _fftBinReady = true;
        }
    }
}
