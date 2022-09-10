#ifndef _COLOR_MATH_H_
#include <colorMath.h>
#endif

template< class T> constantChangeRater<T>::constantChangeRater( T changeRate, T changeTime)
: unit( changeRate )
, changeTime( changeTime )
{
    stepSize = changeRate / changeTime;
}

template< class T> T constantChangeRater<T>::limit( T value )
{
    //Serial.print("Input: ");
    //Serial.print( value ); 

    // Value wich tells how many steps of changetime we're going
    T timeSteps = ( millis() - lastTime ) / changeTime;

    if ( timeSteps > 1.0 ) timeSteps = 2;

    // lets calculate how many changeRates we want to go
    T changes = (value - lastValue) / unit;

    if (timeSteps < 1.0 )
    {     
        changes = constrain( changes, -1, 1);
    } else {
        if (changes < 0) changes = -1;
        else changes = 1;
        
        changes *= timeSteps;
    }

    //if (changes > 1.0 && timeSteps < 1.0) changes = 1;
    //else if (changes < -1.0) changes = -1;
    /*
    Serial.print(F("  TimeSteps: "));
    Serial.print(timeSteps,5);
    Serial.print(F("  Changes: "));
    Serial.print(changes,3);
    */

    lastValue += unit * changes * timeSteps;
    /*
    Serial.print(F("  plus: "));
    Serial.print( lastValue - value);
    */
    /*
    if ( changes >= 1.0 && timeSteps <= 1.0 ) value += changeRate * timeSteps;
    else value += changeRate * changes;
    */
    lastTime = millis();
    /*
    Serial.print(F("  Output: "));
    Serial.println( lastValue );
    Serial.flush();
    */
    return lastValue;
}

template<class T> constantChangeRater<T>::~constantChangeRater()
{
}




ledControl::ledControl()
{
}

void ledControl::Init(CFastLED* fastLedObj, ledStrip* strip)
{
    _FastLED = fastLedObj;
    _strip = strip;
}

void ledControl::logTime()
{
    _lastTime = micros();
}

inline void ledControl::logLastValue(uint8_t hue, uint8_t saturation, uint8_t value)
{
    _lastHue = hue;
    _lastSaturation = saturation;
    _lastValue = value;
}

bool ledControl::Fade(uint8_t hue,uint8_t brightness)   
{
    float val = bright2.calc( bright.calc(brightness) );                   // Calculate the smoothing
    float colorVal = color.limit(hue);
    
    
    if ( colorVal > 255 ) colorVal = 255;
    if ( colorVal < 0 ) colorVal = 0;

    fill_solid( _strip->ledArr, _strip->arrSize, ColorFromPalette( _strip->colorPalette, (uint8_t) colorVal,(uint8_t) val, LINEARBLEND ));   // Sets whole strip to same color using a color palette
    //fill_solid(_strip->ledArr, _strip->arrSize, CHSV( (uint8_t) colorVal, 255, (uint8_t) val ));   // Sets whole strip to same color using a color palette


    /* Check if any rgb values have changed */
    if ( _strip->ledArr->r == _lastHue && _strip->ledArr->g == _lastSaturation && _strip->ledArr->b == _lastValue ) return 0;   // if no value changed return 0
    //fill_solid( _strip->ledArr, _strip->arrSize, _strip->ledArr[0] );

    /*
    Serial.print(F("id: "));
    Serial.print(colorVal);

    Serial.print(F("  br: "));
    Serial.print(val);

    Serial.print(F(" r: "));
    Serial.print( _strip->ledArr->r );
    
    Serial.print(F("  g: "));
    Serial.print( _strip->ledArr->g );

    Serial.print(F("  b: "));
    Serial.println( _strip->ledArr->b );

    Serial.flush();
    */

    logLastValue(_strip->ledArr->r, _strip->ledArr->g, _strip->ledArr->b);                       // save the last hsv values

    return 1;
}

ledControl::~ledControl()
{
}