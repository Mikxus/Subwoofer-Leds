/*
 * MIT License
 *
 * Copyright (c) 2023 Mikko Johannes Heinänen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_

#include "../debug.h"
#include "storage_strategy.h"
#include "eeprom_backend.h"

#if PERSISTENCE_BACKEND == EEPROM
// lass storage_backend_template& persistence_backend = eeprom_storage_backend(0, 500, 0);

storage_backend_template &&persistence_backend = eeprom_storage_backend(0, 500, 0);
#else
#warning "No storage persistence_backend is used"
storage_backend_template &persistence_backend;
#endif

class storage
{
private:
    bool validate_index(uint16_t &index);

public:
    /**
     * @brief Reads byte from the storage backend
     *
     * @param pos
     * @return uint8_t
     */
    uint8_t read(uint16_t index);

    /**
     * @brief Writes byte to storage backend
     *
     * @param index
     * @param input
     */
    void write(uint16_t index, uint8_t input);

    /**
     * @brief Updates value if different
     *
     * @param index
     * @param value
     * @return * void
     */
    void update(uint16_t index, uint8_t input);

    template <typename T>
    T &get(uint16_t index, T &obj)
    {
        if (!validate_index(index))
            return obj;

        return persistence_backend.get(index, &obj);
    }

    template <typename T>
    T &put(uint16_t index, T &obj)
    {
        if (!validate_index(index))
            return obj;

        return persistence_backend.get(index, &obj);
    }
};

extern storage settings_storage;

#endif