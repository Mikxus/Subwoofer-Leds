# rISR version 1.1.0

rISR (Runtime Interrupt Service Routine) is a C library that allows you to bind interrupt vectors at runtime on the atmega328p chip. It is designed to be easy to use and provides a simple API for binding and un-binding ISRs (Interrupt Service Routines) to interrupt vectors.

## Features
- Bind & unbind interrupt vectors at runtime
- Ability to add custom data pointers to each interrupt vector
- Fully supports Atmega328p's interrupts
- Arduino support

## How to use
### Prerequisites
Before you can use the rISR library, you will need to have the following installed on your machine:
- [git](https://scm.com)
- [avr toolchain](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)

Or if you prefer Arduino framework:

- [Arduino ide](https://www.arduino.cc/en/software) / [Arduino cli](https://arduino.github.io/arduino-cli/latest/#installation)

### Installation
- Clone the rISR repo
```
git clone https://github.com/Mikxus/rISR.git
```

Or If you're using Arduino, you can also install the library by following these steps:

1. Go to the [releases tab](https://github.com/Mikxus/rISR/releases) on the rISR repository
2. Download the latest version of the library as a .zip file
3. Open the Arduino IDE
4. Go to Sketch > Include Library > Add .ZIP Library
5. Select the downloaded .zip file and click open
6. The library should now be installed and ready to use in the Arduino IDE.

### Example project
Next create new main.c or a sketch and copy over the example.
```C
#include <stdbool.h>
#include <rISR.h>

__attribute__ ((signal)) void __vector_pin_blink()
{
    PORTB ^= 0b00010000;
    return;
}

int main() {

    /* Configure timer1 to interrupt as fast as possible */
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    PRR &= ~(1 << PRTIM1);
    OCR1A = 1;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    TIMSK1 |= (1 << OCIE1B);

    /* Set pin PB4 to output */
    /* Pin 12 on arduino uno r3 */ 
    DDRB = 0b00010000;                    

    /* Bind isr to selected interrupt vector */
    bind_isr(TIMER1_COMPB_, __vector_pin_blink);
    sei();

    while (true){}
}
```
**Note** if you're using arduino. Change the ```int main()``` to ```void loop()```

Then new create config.h file or edit rISR's config file. The config file should have the interrupt name uncommented to enable runtime binding for it.
```C
#define TIMER1_COMPB_used
```

#### Compilation
- The library consists of header file, c file and a assembly file. These files need to have the interrupt definition config file included. To do this you'll need to add ```-include path/to/config.h``` to avr-gcc's input.

#### Example makefile
Create new Makefile and copy the following content to it, while replacing "path_to_" with the path to the rISR src folder.

```Makefile
CC := avr-gcc
CFLAGS := -Wall --param=min-pagesize=0
BOARD := atmega328p
BUILD_DIR := build

.PHONY: clean

main.c: $(BUILD_DIR)
    $(CC) -mmcu=$(BOARD) $(CFLAGS) $@ -include config.h path_to_rISR.c path_to_rISR.S -o $(BUILD_DIR)/main.o

$(BUILD_DIR):
    mkdir $(BUILD_DIR)

clean:
    rm -rf $(BUILD_DIR)
```

To build the example you can just run ```make```

## API reference
Bind an ISR to an interrupt vector.
```C
void bind_isr(enum isr_vectors vector, vector_t isr);
```

Un-bind an ISR from an interrupt vector.
```C
void unbind_isr(enum isr_vectors vector);
```

Bind data pointer to interrupt vector.
```C
void bind_isr_data_ptr(enum isr_vectors vector, void* pointer);
```
### APIs for passing data pointers
**Note** the vector names for data pointers end in a _ptr suffix.

Example:
```C
TIMER1_COMPB_ptr
```

Unbind data pointer from a interrupt vector
```C
void unbind_isr_data_otr(enum isr_data_pointers vector, void* pointer);
```

Get binded data pointer  
```C
void *get_isr_data_ptr(enum isr_data_pointers vector);
```

## License
This library is licensed under the MIT License. See [LICENSE](https://github.com/Mikxus/rISR/blob/main/LICENSE) file for more information.
