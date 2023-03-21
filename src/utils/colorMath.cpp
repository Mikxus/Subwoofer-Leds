#include "colorMath.h"

float moving_average_filter::calc(float val)
{
    _sampleTotal = _sampleTotal - _window[_curReadIndex];   // subtract the last reading
    _window[_curReadIndex] = val;                           // add the latest value
    _sampleTotal += _window[_curReadIndex];                  // add the reading to the total
    _sampleAvg = _sampleTotal / 2.0;                          // calculate the average
    _curReadIndex += 1;
    if ( _curReadIndex >= 2 ) _curReadIndex = 0;
    return _sampleAvg;
}

// Create function that takes in a float then smooths it, with given smoothing factor
// comment the output range of the function
float weighted_moving_average_filter::calc(float analogRead)
{
    _result = _Alpha * analogRead + (1 - _Alpha) * _result;
    return _result;
    // output range: 0.0 to 1.0
}