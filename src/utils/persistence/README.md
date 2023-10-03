# Persistence Backend Template

The `persistence_backend_template` class provides a template for implementing a persistence backend that mimics the Arduino EEPROM API. This class can be used to read from and write data to a persistence storage.

## Usage

To use the `persistence_backend_template` class, you need to create a derived class that implements the virtual `read` and `write` methods. These methods should specify how data is read from and written to the persistence storage.

Here is an example derived class called `eeprom_persistence_backend` that uses the Arduino EEPROM library to read from and write to the EEPROM on an Arduino board:

```c++
#include <EEPROM.h>
#include "persistence_strategy.h"

class eeprom_persistence_backend : public persistence_backend_template
{
public:
    eeprom_persistence_backend(uint8_t* min, uint8_t* max, uint8_t* pos)
        : persistence_backend_template(min, max, pos)
    {
    }

    void write(uint8_t* index, uint8_t input) override
    {
        EEPROM.write(index - m_min_pos, input);
    }

    uint8_t read(uint16_t index) override
    {
        return EEPROM.read(index - m_min_pos);
    }
};
```

This derived class overrides the virtual `read` and `write` methods of the `persistence_backend_template` class. The `write` method uses the `EEPROM.write()` function to write data to the specified index in the EEPROM. The `read` method uses the `EEPROM.read()` function to read data from the specified index in the EEPROM.

To use this derived class in your code, you can create an instance of it and use it like this:


Here is an example README that explains how to use the `persistence_backend_template` class and how to implement a custom backend:

# Persistence Backend Template

The `persistence_backend_template` class provides a template for implementing a persistence backend that mimics the Arduino EEPROM API. This class can be used to read from and write data to a persistence storage.

## Usage

To use the `persistence_backend_template` class, you need to create a derived class that implements the virtual `read` and `write` methods. These methods should specify how data is read from and written to the persistence storage.

Here is an example derived class called `eeprom_persistence_backend` that uses the Arduino EEPROM library to read from and write to the EEPROM on an Arduino board:

```c++
#include <EEPROM.h>
#include "persistence_strategy.h"

class eeprom_persistence_backend : public persistence_backend_template
{
public:
    eeprom_persistence_backend(uint8_t* min, uint8_t* max, uint8_t pos)
        : persistence_backend_template(min, max, pos)
    {
    }

    void write(uint8_t* index, uint8_t input) override
    {
        EEPROM.write(index - m_min_pos, input);
    }

    uint8_t read(uint16_t index) override
    {
        return EEPROM.read(index - m_min_pos);
    }
};
```

This derived class overrides the virtual `read` and `write` methods of the `persistence_backend_template` class. The `write` method uses the `EEPROM.write()` function to write data to the specified index in the EEPROM. The `read` method uses the `EEPROM.read()` function to read data from the specified index in the EEPROM.

## Implementing a Custom Backend

To implement a custom backend, you need to create a derived class that implements the virtual `read` and `write` methods. These methods should specify how data is read from and written to your custom persistence storage.

Here is an example derived class called `my_persistence_backend` that uses a custom persistence storage:

```c++
#include "persistence_strategy.h"

class my_persistence_backend : public persistence_backend_template
{
public:
    my_persistence_backend(uint8_t* min, uint8_t* max, uint8_t pos)
        : persistence_backend_template(min, max, pos)
    {
    }

    void write(uint8_t* index, uint8_t input) override
    {
        // Implement writing to your custom persistence storage here
    }

    uint8_t read(uint16_t index) override
    {
        // Implement reading from your custom persistence storage here
        return 0;
    }
};
```

This derived class overrides the virtual `read` and `write` methods of the `persistence_backend_template` class. You need to implement these methods to specify how data is read from and written to your custom persistence storage.