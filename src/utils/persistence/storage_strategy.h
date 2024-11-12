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
#ifndef _STORAGE_STRATEGY_H_
#define _STORAGE_STRATEGY_H_

#include <math.h>
#include <inttypes.h>
#include "../../config.h"

/**
 * @brief Storage backend template. Mimics arduino's eeprom api
 *
 */
class storage_backend_template
{
protected:
    uint16_t m_pos;

public:
    uint16_t m_min_pos;
    uint8_t m_max_pos;

    /**
     * @brief Construct a new storage backend template
     *
     * @param min Start address of persistence storage
     * @param max End address of persistence storage
     * @param pos Position of the write head
     */
    storage_backend_template(uint16_t min, uint16_t max, uint16_t pos)
        : m_pos(pos), m_min_pos(min), m_max_pos(max)
    {
    }

    virtual ~storage_backend_template() = default;

    /**
     * @brief Get the mem size of storage storage
     *
     * @return uint16_t
     */
    uint16_t get_mem_size()
    {
        int32_t temp = m_max_pos - m_min_pos;
        return (uint16_t) abs(temp);
    }

    /**
     * @brief Writes byte
     *
     * @param index
     * @param input
     */
    virtual void write(uint16_t index, uint8_t input) = 0;

    /**
     * @brief Reads a byte
     *
     * @param index
     * @return uint8_t
     */
    virtual uint8_t read(uint16_t index) = 0;

    /**
     * @brief Writes the input if values differ
     *
     * @param index
     * @param input
     */
    void update(uint16_t index, uint8_t input)
    {
        if (input == *this)
            return;

        write(index, input);
    }

    template <typename T>
    T &get(uint16_t index, T &obj)
    {
        uint8_t *obj_ptr = (uint8_t *)&obj;
        for (uint32_t i = 0; i < sizeof(T); i++)
        {
            obj_ptr[i] = read(index + i);
        }

        return obj;
    }

    template <typename T>
    const T &put(uint16_t index, const T &obj)
    {
        const uint8_t *obj_ptr = (const uint8_t *)&obj;
        for (uint32_t i = 0; i < sizeof(T); i++)
        {
            write(index + i, obj_ptr[i]);
        }

        return obj;
    }

    /* operators */
    uint8_t operator*() const { return read(m_pos); }
    operator uint8_t() const { return **this; }

    /* Postfix increment */
    uint8_t operator++(int)
    {
        uint8_t temp = **this;
        return ++(*this), temp;
    }

    /* Postfix decrement */
    uint8_t operator--(int)
    {
        uint8_t temp = **this;
        return --(*this), temp;
    }

    /* Prefix increment */
    storage_backend_template &operator++()
    {
        return *this += 1;
    }

    /* Prefix decrement */
    storage_backend_template &operator--()
    {
        return *this -= 1;
    }

    storage_backend_template &operator=(const storage_backend_template &ref) { return *this = *ref; }
    storage_backend_template &operator=(uint8_t input) { return write(m_pos, input), *this; }
    storage_backend_template &operator+=(uint8_t input) { return *this = **this + input; }
    storage_backend_template &operator-=(uint8_t input) { return *this = **this - input; }
    storage_backend_template &operator*=(uint8_t input) { return *this = **this * input; }
    storage_backend_template &operator/=(uint8_t input) { return *this = **this / input; }
    storage_backend_template &operator^=(uint8_t input) { return *this = **this ^ input; }
    storage_backend_template &operator%=(uint8_t input) { return *this = **this % input; }
    storage_backend_template &operator&=(uint8_t input) { return *this = **this & input; }
    storage_backend_template &operator|=(uint8_t input) { return *this = **this | input; }
    storage_backend_template &operator<<=(uint8_t input) { return *this = **this << input; }
    storage_backend_template &operator>>=(uint8_t input) { return *this = **this >> input; }
};

#endif