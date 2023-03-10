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
#ifndef _FFT_STRATEGY_H_
#define _FFT_STRATEGY_H_

#include "../lib/rISR/src/rISR.h"

/**
 * @brief Enum for implemented backends
 * 
 */
typedef enum {
    fixed_8,
} fft_backend;

/**
 * @brief FFT abstraction layer
 * 
 */
class FFT_backend_template
{
protected:
    uint16_t m_sample_size;
    void *m_data = nullptr;

    /**
     * @brief Allocates array of custom data size * sample size. Then sets data to point to it
     * 
     * @return true Allocation succesfull
     * @return false Allocation failed
     */
    virtual bool allocate_data_array();

    virtual void deallocate_data_array();

public:
    uint32_t m_sampling_frequency = 0.0F;
    
    /**
     * @brief Construct a new fft backend template object
     * @note !! constructor has to call allocate_data_array
     * 
     * @param sample_size 
     * @param frequency 
     * @param bits 
     */
    FFT_backend_template( uint8_t input_pin, uint16_t sample_size, uint16_t frequency, fft_backend bits )
    : m_sample_size(sample_size)
    {
    }

    virtual bool set_sample_size(uint16_t sample_size);

    /**
     * @brief Calculates fft & returns the loudest hz.
     * 
     * @return uint16_t 
     */
    virtual uint16_t calculate() = 0;
    
    /**
     * @brief Gets the fft implementation specific sampling interrupt vector address.
     * 
     * @return void*
     */
    virtual vector_t get_read_vector() = 0;

    /**
     * @brief Get the read vector data pointer
     * 
     * @return void* 
     */
    virtual void *get_read_vector_data_pointer() = 0;
    
    virtual ~FFT_backend_template() = default;
};

#endif