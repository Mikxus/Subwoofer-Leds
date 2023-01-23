# SubEffects 0.2.0v
This library intends to provide an easy way to create and use light effects with different led strips. You can find information for the led chips supported by [FastLED](https://github.com/FastLED/FastLED#supported-led-chipsets) library.
>**! Library is still under development. Many things will be added or removed.**

# Light modes
**Bass color**
>**Index 0**
> In this mode the color is changed by the bass frequencies. The brightness is changed by the loudness of signal.
> The bass frequency detection is implemented by using [arduinoFFT](https://github.com/kosme/arduinoFFT) library.
> **! Note** this mode uses the **timer1** to collect samples of the audio.

# How to use


# API

**Useful for just using the library.**

* **SubEffects**( uint8_t audioPin, uint8_t led_dataPin, **CFastLED** & fastLedObj, **CRGB** ledObj[ ] );
   >Used to initialize the library.
   > Takes the following arguments. 
   > 1. Analog pin's number 2.  Led's Data pin 3. FastLED object 4. CRGB led array

* void **NextMode**( );
  > Increments the lighting mode by 1. 
  > **!** Overflows if exceedes the lighting mode count.
* void **PreviusMode**( );
  > Decrements the lighting mode by 1.
  > **!** Overflows to highest lighting mode.
 
* void **SetMode**( uint8_t mode );
  > Sets the lighting mode to the corresponding number.
  > **!** Allowed values 0 - 255

* void **CalibrateSoundLevel**( );
  >Calibrates for the dc offset. The calibration takes 1.5 seconds.
  >Useful for preventing the leds blinking randomly.

    ## [Advanced APIs](/docs/api/apis_for_light_modes.md)
     
# License

MIT License

Copyright (c) 2021 Mikko Johannes Heinänen 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
