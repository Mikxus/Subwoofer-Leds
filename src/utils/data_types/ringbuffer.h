/*
 * MIT License
 *
 * Copyright (c) 2024 Mikko Johannes Heinänen
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
#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "../../config.h"
#include "../debug.h"


/**
 * @brief Simple ringbuffer implementation
 * 
 * @tparam T 
 */
template<typename T>
class ringbuffer
{
private:
    T* buffer;
    uint16_t size = 0;
    uint16_t head = 0;
    uint16_t tail = 0;

protected:

    /**
     * @brief Increments head position
     * 
     */
    void increment_head_position()
    {
        head = (head + 1) % size;

        /* Case: buffer is full
         * need to move the tail as well 
         */
        if (get_used_size() == size) 
        {
            tail = (tail + 1) % size;
        }
    }

    /**
     * @brief Decrements head position
     * 
     */
    void decrement_head_position()
    {
        int32_t _head = head;
        int32_t _tail = tail;
        int32_t _size = size;

        if (_head == _tail)
        {
            WARN(F("ringbuffer: can't decrement head. Buffer is empty"));
            return;
        }

        head = (_head - 1L) % _tail;
    }

    /**
     * @brief Decrements tail position ie removes element from the tail
     * 
     */
    void decrement_tail_position()
    {
        if (get_used_size() == 0)
        {
            WARN(F("ringbuffer: can't decrement tail. Buffer is empty"));
            return;
        }

        if (tail == size)

        tail = (tail + 1) % size;

    }

public:
    ringbuffer(T* array, uint16_t size) {resize(array, size);}

    void resize(T* array, uint16_t array_size)
    {
        if (array == nullptr)
        {
            ERROR("ringbuffer: array is nullptr");
            return;
        }

        buffer = array;
        size = array_size;
        head = 0;
        tail = 0;
    }

    /**
     * @brief Returns the used size 
     * 
     * @tparam T 
     * @return uint16_t 
     */
    uint16_t get_used_size()
    {
        /* using 32bit unsigned numbers to prevent overflows/underflows */
        int32_t _head = head;
        int32_t _tail = tail; 
        int32_t _size = size;

        return ( _head - _tail + _size) % _size;
    }

    /**
     * @brief Inserts an element at the buffer's head.
     * 
     * @tparam T 
     */
    void push(T value)
    {
        buffer[head] = value;
        increment_head_position();
    }

    T pop()
    {

    }

    /**
     * @brief Removes the oldest element in the buffer
     * 
     * @tparam T 
     * @return T 
     */
    T pop_tail()
    {
        T value;

        if (get_used_size() == 0)
        {
            ERROR(F("ringbuffer: buffer is empty"));
            return value;
        }

        value = buffer[tail];
        tail = (tail + 1) % size;
    }

};

#endif