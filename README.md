# SubEffects v0.2.0
```Note that this library is still work in progress. Many things will change and break.```

SubEffects is an Arduino library that makes creating dynamic, audio-responsive light effects easy. With it's hassle-free FFT implementation and user-friendly API, you can quickly add and control multiple addressable LED strips, set brightness, change effects, and color palettes.

## Features
- Compatible with Arduino uno, Nano and other Atmega328p based boards
- Support for many addressable leds since SubEffects uses [FastLED](https://github.com/FastLED/FastLED) library to interface with the leds
- Supports color palettes
- Easy to use 8bit fixed point FFT [implementation](https://github.com/Klafyvel/AVR-FFT/tree/main/Fixed8FFT)

## Installation

1. Go to the [releases tab](https://github.com/Mikxus/rISR/releases) on the rISR repository
2. Download the latest version of the library as a .zip file
3. Open the Arduino IDE
4. Go to Sketch > Include Library > Add .ZIP Library
5. Select the downloaded .zip file and click open
6. The library should now be installed and ready to use in the Arduino IDE.

Once the library is installed, you can include it in your sketch by adding the following line at the top of your sketch:
```C++
#include <SubEffects.h>
```
## Usage

The library includes a base class, SubEffects, that you can use to control the LED strips. The SubEffects class includes several methods that you can use to add LED strips, set the global brightness, set the global mode, and set the global color palette.

To use the SubEffects class, you will need to create an instance of the class, passing in a pointer to the FastLED object:
```
SubEffects effect(&FastLED);
```

Once you have created an instance of the SubEffects class, you can add LED strips using the AddLedStrip method. The AddLedStrip method takes three arguments: the pin number of the audio input, the LED mode, and the number of LEDs in the strip. For example:
```C++
effect.AddLedStrip(A0, LED_MODE_AUDIO_AVG, 60);
```

You can also set the global brightness using the SetBrightness method:
```C++
effect.SetBrightness(255);
```

You can also set the global mode using the SetMode method:
```C++
effect.SetMode(0);
```

You can also set the global color palette using the SetPalette method:
```C++
effect.SetPalette(0);
```

For more detailed information on how to use the library, please look at the example sketches.

## Limitations

## License
This library is released under the MIT License. See the [LICENSE](https://github.com/Mikxus/SubEffects/blob/dev/LICENSE) file for more details.
