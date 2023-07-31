#ifndef _FIXED_8_FFT_H_
#define _FIXED_8_FFT_H_

#include <inttypes.h>
#include <math.h>
#include <FastLED.h>
#include "../../utils/debug.h"
#include "../../utils/interrupt.h"
#include "../../utils/FFT/FFT_strategy.h"
#include "../rISR/src/rISR.h"

typedef int16_t fixed16_t;
typedef int8_t fixed8_t;

extern const fixed16_t FIXED_16_ONE;
extern const fixed16_t FIXED_16_ZERO;
extern const fixed8_t MODULUS_MAGIC;
extern const fixed8_t ONE_OVER_SQRT_TWO;
extern const fixed16_t FIXED_8_ONE;
extern const fixed16_t FIXED_8_ZERO;
extern const fixed16_t FIXED_8_HALF;

extern fixed16_t sines[];
extern fixed16_t two_sines_sq[];

/**
 * @brief Calculates fft for the x array.
 *
 * @param x
 * @param size
 * @return uint8_t
 */
extern uint8_t fft(fixed8_t x[], const int size);

/**
 * @brief Reverses specified bit?
 *
 * @param nbits
 * @param val
 * @return uint8_t
 */
extern uint8_t bit_reverse(const uint8_t nbits, uint8_t val);

extern fixed8_t fixed16_to_fixed8(fixed16_t x);

/**
 * @brief Signed fractional multiply of two 8-bit numbers.
 *
 * @param a
 * @param b
 * @return fixed8_t
 */
extern fixed8_t fixed_mul_8_8(fixed8_t a, fixed8_t b);

/**
 * @brief Signed fractional multiply of two 16-bit numbers with 16-bit result.
 *
 * @param a
 * @param b
 * @return fixed16_t
 */
extern fixed16_t fixed_mul_16_16(fixed16_t a, fixed16_t b);

/* Overloading utilities */
extern fixed8_t fixed_mul_8_16(fixed8_t a, fixed16_t b);

extern fixed8_t fixed_mul_16_8(fixed16_t a, fixed8_t b);

extern fixed8_t fixed_add_saturate_8_16(fixed8_t a, fixed16_t b);

extern fixed8_t fixed_add_saturate_16_8(fixed16_t a, fixed8_t b);

/**
 * @brief Approximate modulus with a 5% margin of error.
 *
 * @param x
 * @param size
 * @param frequency
 * @return * float
 */
extern uint16_t modulus(fixed8_t x[], const int size, float frequency);

/**
 * @brief fixed point addition with saturation to ±1.
 *
 * @param a
 * @param b
 * @return fixed8_t
 */
extern fixed8_t fixed_add_saturate_8_8(fixed8_t a, fixed8_t b);

/**
 * @brief Fixed point addition with saturation ±1.
 *
 * @param a
 * @param b
 * @return fixed16_t
 */
extern fixed16_t fixed_add_saturate_16_16(fixed16_t a, fixed16_t b);

/**
 * @brief Isr for reading 8bit adc value using timer1 compb interrupt
 *
 */
extern void __vector_timer1_compb_adc_read_byte();

#ifndef _FIXED8FFT_ADC_SAMPLE_INTERRUPT_STRUCT_
#define _FIXED8FFT_ADC_SAMPLE_INTERRUPT_STRUCT_

/**
 * @brief Exapmle adc read interrupt data structure
 *
 */
struct adc_sample_interrupt
{
    struct
    {
        volatile uint32_t adc_pin : 4;    // Adc input pin
        volatile uint32_t array_size : 8; // data array size in power of 2. Max size 256
        volatile uint32_t array_pos : 9;  // array pos. max size 512

        /* Values to scale the input data */
        volatile uint32_t offset_x : 7;
        volatile uint32_t scale_x : 5;
    };

    /* pointer to int8_t array */
    int8_t *volatile data;
};
#endif

/* Concrete strategy class for 8bit fft */
class Fixed8FFT : public FFT_backend_template
{
private:
    adc_sample_interrupt interrupt_data;
    uint32_t last_result_time = 0;

    /**
     * @brief Calculates scaling values for the interrupts
     *
     */
    void calculate_scaling();
    uint8_t get_power_of_two(uint16_t value);

protected:
    bool allocate_data_array() override;
    void deallocate_data_array() override;

public:
    Fixed8FFT(uint8_t input_pin, uint16_t sample_size, uint16_t frequency, fft_backend bits);
    uint16_t calculate() override;

    bool set_sample_size(uint16_t sample_size) override;
    vector_t get_read_vector() override;
    void *get_read_vector_data_pointer() override;
    ~Fixed8FFT();
};
#endif