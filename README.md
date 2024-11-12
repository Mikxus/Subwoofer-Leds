# SubEffects v0.3.0
```Note that this library is one of my hobby projects. The goal posts may be moved eratically and development continues when i feel like doing it.```

SubEffects is an Arduino library that makes creating dynamic, audio-responsive and managing light effects easier. This library is intended to provide simple api to handle creating different effects on a led strip and provides some rudimentary ways to hande the "audio resposive" side of things.

## Features
- Compatible with Arduino uno, Nano and other Atmega328p based boards
- Support for many addressable leds since SubEffects uses [FastLED](https://github.com/FastLED/FastLED) library to interface with the leds
- Easy to use 8bit fixed point FFT [implementation](https://github.com/Klafyvel/AVR-FFT/tree/main/Fixed8FFT)

## Installation
1. Go to the [tags tab](https://github.com/Mikxus/SubEffects/tags)
2. Download the latest version of the library as a .zip file
3. Open the Arduino IDE
4. Click on the toolbar Sketch > Include Library > Add .ZIP Library
5. Select the downloaded .zip file and click open
6. The library should now be installed and ready to use in the Arduino IDE.

Once the library is installed, you can include it in your sketch by adding the following line at the top of your sketch:
```C++
#include <SubEffects.h>
```
## How does this work?

This library abstracts the driving of leds and effects to three different "objects"
- ```C++
  class led_manager
  ```
  led_manager is used to control different physical led strips. Which contain the effects for the led strip.
  
- ```C++
  struct ledStrip
  ```
  Representation of a single physical led strip.

- ```C++
  class audioMode
  ```
  Abstract class for a audio effect.


## Limitations

## License
This library is released under the MIT License. See the [LICENSE](https://github.com/Mikxus/SubEffects/blob/dev/LICENSE) file for more details.
