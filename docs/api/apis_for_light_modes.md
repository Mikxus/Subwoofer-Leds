# API

  

## fft.h
An interface for the ardunoFFT library.
#### Public APIs
* bool **Init**( );
   >Initializes the library in following order:
   >1.   Allocates the bins needed for fft.
   >2. Configures the arduino's timer1 to interrupt at default frequency.
   >
   >**Returns:** 0 when allocation fails, otherwise 1.
*   void **Stop**( );
    > Stops the audio sampling and deallocates the fft bins.

* bool **SetSampleSize**( uint16_t size )
     > If nothing entered it uses 64 as the default value.
     >Changes the bin size for the fft.
     > **Returns:** 0 if there isn't enough memory otherwise returns 1.
     > **! Notes !**
    > *  ****Sample size must be power of 2.****
    
 * void **SetFrequency**( uint16_t Hz );
    > Sets the sample rate to specified frequency.
    > **! note** there is an upper limit to the frequency, but the code **does not check it**.

* uint16_t **Calculate**( );
  > Calculates the fft if bins are ready.
  > **Returns:** strongest hz

* void **Benchmark**( )
  > Allocates the bins then fills them with values and performs fft on them.

#### Private APIs
* bool **allocMem**( );
  >Allocates the fft bins.
  >**Returns:** 0 if allocation failed otherwise 1.
* void **deallockMem**( );
  > Deallocates the fft bins.


## timer1.h
   Library wich implements a simple way to enable interrupts at specified intervall. **! Works closely with fft.h**. Might join these two libraries at somepoint.

#### Protected APIs
* void **Start**( uint16_t size, uint16_t Hz )
* void **Stop**( );