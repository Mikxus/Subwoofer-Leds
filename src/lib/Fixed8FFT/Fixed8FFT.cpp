#include "Fixed8FFT.h"

/* Const values */
const fixed16_t FIXED_16_ONE = 0x7fff;
const fixed16_t FIXED_16_ZERO = 0x0000;
const fixed8_t MODULUS_MAGIC = 0x03;
const fixed8_t ONE_OVER_SQRT_TWO = 0x5b;
const fixed16_t FIXED_8_ONE = 0x7f;
const fixed16_t FIXED_8_ZERO = 0x00;
const fixed16_t FIXED_8_HALF = 0x40;

/* Sine values */
fixed16_t sines[] = {
    0x0000, // sin(-π)
    0x8000, // sin(-π/2)
    0xa57e, // sin(-π/4)
    0xcf04, // sin(-π/8)
    0xe707, // sin(-π/16)
    0xf374, // sin(-π/32)
    0xf9b8, // sin(-π/64)
    0xfcdc, // sin(-π/128)
    0xfe6e  // sin(-π/256)
};

fixed16_t two_sines_sq[] = {
    0x7fff, // 2sin²(-π/2)
    0x7fff, // 2sin²(-π/4)
    0x257e, // 2sin²(-π/8)
    0x09be, // 2sin²(-π/16)
    0x0277, // 2sin²(-π/32)
    0x009e, // 2sin²(-π/64)
    0x0028, // 2sin²(-π/128)
    0x000a  // 2sin²(-π/256)
};

uint8_t fft(fixed8_t x[], int size)
{
    if (size == 1)
        return 0;

    /* indices */
    uint8_t i, j, k, n_1, array_num_bits;
    uint8_t n_2 = 1;
    /* temporary buffers that should be used right away. */
    fixed8_t tmp, a, b, c, d, k1, k2, k3;
    /* Will store angles, and recursion values for cosine calculation */
    fixed16_t alpha, beta, cj, sj, tmp_trigo;
    uint8_t scale = 0;

    uint8_t half_size = size >> 1;

    /* How many bits we need to store the positions in half the array.
       This switch could be replaced by a const variable to gain a bit
       of space if we were always computing on the same size of arrays.
    */

    switch (size)
    {
    case 2:
        array_num_bits = 0;
        break;
    case 4:
        array_num_bits = 1;
        break;
    case 8:
        array_num_bits = 2;
        break;
    case 16:
        array_num_bits = 3;
        break;
    case 32:
        array_num_bits = 4;
        break;
    case 64:
        array_num_bits = 5;
        break;
    case 128:
        array_num_bits = 6;
        break;
    case 256:
        array_num_bits = 7;
        break;
    default:
        array_num_bits = 0;
        break;
    }

    /* Reverse-bit ordering */
    for (i = 0; i < half_size; ++i)
    {
        j = bit_reverse(array_num_bits, i);

        if (i < j)
        {
            /* Swapping real part */
            tmp = x[i << 1];
            x[i << 1] = x[j << 1];
            x[j << 1] = tmp;
            /* Swapping imaginary part */
            tmp = x[(i << 1) + 1];
            x[(i << 1) + 1] = x[(j << 1) + 1];
            x[(j << 1) + 1] = tmp;
        }
    }

    /* Actual FFT */
    for (i = 0; i < array_num_bits; ++i)
    {
        /* n_1 gives the size of the sub-arrays */
        n_1 = n_2; // n_1 = 2^i
        /* n_2 gives the number of steps required to go from one group of sub-arrays to another */
        n_2 = n_2 << 1; // n_2 = 2^(i+1)

        alpha = two_sines_sq[i];
        beta = sines[i];
        /* Those two will store the cosine and sine 2pij/n₂ */
        cj = FIXED_16_ONE;
        sj = FIXED_16_ZERO;

        /* Scale down the array of data before the pass, to ensure no overflow happens. */
        for (j = 0; j < half_size; j++)
        {
            x[2 * j] /= 2;     // fixed_mul_8_8(x[2*j], FIXED_8_HALF);
            x[2 * j + 1] /= 2; // fixed_mul_8_8(x[2*j+1], FIXED_8_HALF);
        }

        /* j will be the index in Xe and Xo */
        for (j = 0; j < n_1; j++)
        {
            /* We combine the jth elements of each group of sub-arrays */
            for (k = j; k < half_size; k += n_2)
            {
                /* Now we calculate the next step of the fft process, i.e.
                   X[j] = Xᵉ[j] + exp(-2im*pi*j/n₂) * Xᵒ[j]
                   X[j+n₂/2] = Xᵉ[j] - exp(-2im*pi*j/n₂) * Xᵒ[j]
                */
                a = x[k << 1];
                b = x[(k << 1) + 1];
                c = x[(k + n_1) << 1];
                d = x[((k + n_1) << 1) + 1];
                x[k << 1] = (a + (fixed_mul_16_8(cj, c) - fixed_mul_16_8(sj, d)));
                x[(k << 1) + 1] = (b + (fixed_mul_16_8(sj, c) + fixed_mul_16_8(cj, d)));
                x[(k + n_1) << 1] = (a + (-fixed_mul_16_8(cj, c) + fixed_mul_16_8(sj, d)));
                x[((k + n_1) << 1) + 1] = (b - (fixed_mul_16_8(sj, c) + fixed_mul_16_8(cj, d)));
            }
            /* We calculate the next cosine and sine */
            tmp_trigo = cj;
            cj = fixed_add_saturate_16_16(cj, -fixed_add_saturate_16_16(fixed_mul_16_16(alpha, cj), fixed_mul_16_16(beta, sj)));
            sj = fixed_add_saturate_16_16(sj, -fixed_add_saturate_16_16(fixed_mul_16_16(alpha, sj), -fixed_mul_16_16(beta, tmp_trigo)));
        }
    }

    for (j = 0; j < half_size; j++)
    {
        x[2 * j] /= 2;
        x[2 * j + 1] /= 2;
    }
    /* Building the final FT from its entangled version */
    /* Special case n=0 */
    x[0] = fixed_add_saturate_8_8(x[0], x[1]);
    x[1] = FIXED_8_ZERO;

    alpha = two_sines_sq[array_num_bits];
    beta = sines[array_num_bits];
    cj = FIXED_16_ONE;
    sj = FIXED_16_ZERO;
    for (j = 1; j <= (half_size >> 1); ++j)
    {
        /* We calculate the cosine and sine before the main calculation here to compensate for the first
           step of the loop that was skipped.
         */
        tmp_trigo = cj;
        cj = fixed_add_saturate_16_16(cj, -fixed_add_saturate_16_16(fixed_mul_16_16(alpha, cj), fixed_mul_16_16(beta, sj)));
        sj = fixed_add_saturate_16_16(sj, -fixed_add_saturate_16_16(fixed_mul_16_16(alpha, sj), -fixed_mul_16_16(beta, tmp_trigo)));

        a = x[j << 1];
        b = x[(j << 1) + 1];
        c = x[(half_size - j) << 1];
        d = x[((half_size - j) << 1) + 1];
        x[j << 1] = fixed_mul_8_8(
            (a + c) +
                ((fixed_mul_8_16(b, cj) + fixed_mul_8_16(a, sj)) + (fixed_mul_8_16(d, cj) - fixed_mul_8_16(c, sj))),
            FIXED_8_HALF);
        x[(j << 1) + 1] = fixed_mul_8_8(
            (b - d) +
                ((-fixed_mul_8_16(a, cj) + fixed_mul_8_16(b, sj)) + (fixed_mul_8_16(c, cj) + fixed_mul_8_16(d, sj))),
            FIXED_8_HALF);
        x[(half_size - j) << 1] = fixed_mul_8_8(
            (a + c) +
                ((-fixed_mul_8_16(d, cj) + fixed_mul_8_16(c, sj)) - (fixed_mul_8_16(b, cj) + fixed_mul_8_16(a, sj))),
            FIXED_8_HALF);
        x[((half_size - j) << 1) + 1] = fixed_mul_8_8(
            (d - b) +
                ((fixed_mul_8_16(c, cj) + fixed_mul_8_16(d, sj)) + (-fixed_mul_8_16(a, cj) + fixed_mul_8_16(b, sj))),
            FIXED_8_HALF);
    }
    return scale;
}

/* This is a bit uggly and can be replaced efficiently if we
   always have the same size of array.
   */
uint8_t bit_reverse(const uint8_t nbits, uint8_t val)
{
    switch (nbits)
    {
    case 8:
        val = bit_reverse(4, (val & 0xf0) >> 4) | (bit_reverse(4, val & 0x0f) << 4);
        break;
    case 7:
        val = bit_reverse(3, (val & 0x70) >> 4) | (val & 0x08) | (bit_reverse(3, val & 0x07) << 4);
        break;
    case 6:
        val = bit_reverse(3, (val & 0x38) >> 3) | (bit_reverse(3, val & 0x07) << 3);
        break;
    case 5:
        val = bit_reverse(2, (val & 0x18) >> 3) | (val & 0x4) | (bit_reverse(2, val & 0x3) << 3);
        break;
    case 4:
        val = bit_reverse(2, (val & 0xc) >> 2) | (bit_reverse(2, val & 0x3) << 2);
        break;
    case 3:
        val = ((val & 0x4) >> 2) | (val & 0x2) | ((val & 0x1) << 2);
        break;
    case 2:
        val = ((val & 0x2) >> 1) | ((val & 0x1) << 1);
        break;
    default:
        break;
    }
    return val;
}

/* Signed fractional multiply of two 8-bit numbers. */
fixed8_t fixed_mul_8_8(fixed8_t a, fixed8_t b)
{
    fixed8_t result = 0;

    asm(
        "fmuls %[a],%[b]"
        "\n\t"
        "mov %[result],__zero_reg__"
        "\n\t"
        "clr __zero_reg__"
        "\n\t"
        :
        [result] "+r"(result)
        :
        [a] "a"(a), [b] "a"(b));
    return result;
}

/* Signed fractional multiply of two 16-bit numbers with 16-bit result. */
fixed16_t fixed_mul_16_16(fixed16_t a, fixed16_t b)
{
    fixed16_t result;
    asm(
        // We need a register that's always zero
        "clr r2"
        "\n\t"
        "fmuls %B[a],%B[b]"
        "\n\t" // Multiply the MSBs
        "movw %A[result],__tmp_reg__"
        "\n\t" // Save the result
        "mov __tmp_reg__,%B[a]"
        "\n\t"
        "eor __tmp_reg__,%B[b]"
        "\n\t"
        "eor __tmp_reg__,%B[result]"
        "\n\t"
        "fmul %A[a],%A[b]"
        "\n\t" // Multiply the LSBs
        "adc %A[result],r2"
        "\n\t" // Do not forget the carry
        "movw r18,__tmp_reg__"
        "\n\t" // The result of the LSBs multipliplication is stored in temporary registers
        "fmulsu %B[a],%A[b]"
        "\n\t" // First crossed product
               // This will be reported onto the MSBs of the temporary registers and the LSBs
               // of the result registers. So the carry goes to the result's MSB.
        "sbc %B[result],r2"
        "\n\t"
        // Now we sum the cross product
        "add r19,__tmp_reg__"
        "\n\t"
        "adc %A[result],__zero_reg__"
        "\n\t"
        "adc %B[result],r2"
        "\n\t"
        "fmulsu %B[b],%A[a]"
        "\n\t" // Second cross product, same as first.
        "sbc %B[result],r2"
        "\n\t"
        "add r19,__tmp_reg__"
        "\n\t"
        "adc %A[result],__zero_reg__"
        "\n\t"
        "adc %B[result],r2"
        "\n\t"
        "clr __zero_reg__"
        "\n\t"
        :
        [result] "+r"(result)
        :
        [a] "a"(a), [b] "a"(b)
        : "r2", "r18", "r19");
    return result;
}

/* fixed point addition with saturation to ±1.*/
fixed8_t fixed_add_saturate_8_8(fixed8_t a, fixed8_t b)
{
    fixed8_t result = 0;
    asm(
        "clr %[result]"
        "\n\t"
        "add %[result],%[b]"
        "\n\t"
        "add %[result],%[a]"
        "\n\t"
        "brvc fixed8_add_saturate_goodbye"
        "\n\t"
        "subi %[result], 0"
        "\n\t"
        "brmi fixed8_add_saturate_plus_one"
        "\n\t"
        "fixed8_add_saturate_minus_one:"
        "\n\t"
        "ldi %[result],0x80"
        "\n\t"
        "jmp fixed8_add_saturate_goodbye"
        "\n\t"
        "fixed8_add_saturate_plus_one:"
        "\n\t"
        "ldi %[result],0x7f"
        "\n\t"
        "fixed8_add_saturate_goodbye:"
        "\n\t"
        :
        [result] "+r"(result)
        :
        [a] "a"(a), [b] "a"(b));

    return result;
}

/* Fixed point addition with saturation to ±1. */
fixed16_t fixed_add_saturate_16_16(fixed16_t a, fixed16_t b)
{
    fixed16_t result = 0;
    asm(
        "movw %A[result], %A[a]"
        "\n\t"
        "add %A[result],%A[b]"
        "\n\t"
        "adc %B[result],%B[b]"
        "\n\t"
        "brvc fixed_add_saturate_goodbye"
        "\n\t"
        "subi %B[result], 0"
        "\n\t"
        "brmi fixed_add_saturate_plus_one"
        "\n\t"
        "fixed_add_saturate_minus_one:"
        "\n\t"
        "ldi %B[result],0x80"
        "\n\t"
        "ldi %A[result],0x00"
        "\n\t"
        "jmp fixed_add_saturate_goodbye"
        "\n\t"
        "fixed_add_saturate_plus_one:"
        "\n\t"
        "ldi %B[result],0x7f"
        "\n\t"
        "ldi %A[result],0xff"
        "\n\t"
        "fixed_add_saturate_goodbye:"
        "\n\t"
        :
        [result] "+d"(result)
        :
        [a] "r"(a), [b] "r"(b));

    return result;
}

/* Overloading utilities */
fixed8_t fixed_mul_8_16(fixed8_t a, fixed16_t b)
{
    return fixed_mul_8_8(a, fixed16_to_fixed8(b));
}

fixed8_t fixed_mul_16_8(fixed16_t a, fixed8_t b)
{
    return fixed_mul_8_8(fixed16_to_fixed8(a), b);
}

fixed8_t fixed_add_saturate_8_16(fixed8_t a, fixed16_t b)
{
    return fixed_add_saturate_8_8(a, fixed16_to_fixed8(b));
}

fixed8_t fixed_add_saturate_16_8(fixed16_t a, fixed8_t b)
{
    return fixed_add_saturate_8_8(fixed16_to_fixed8(a), b);
}

fixed8_t fixed16_to_fixed8(fixed16_t x)
{
    return (fixed8_t)(x >> 8);
}

/* Approximate modulus with a 5% margin error.
   See here (https://klafyvel.me/blog/articles/approximate-euclidian-norm/)
   for why it works.
   */
uint16_t modulus(fixed8_t x[], int size, float frequency)
{
    uint8_t i, i_maxi = 0;
    fixed8_t a = 0, b = 0;
    fixed8_t maxi = 0;
    for (i = 0; i < size / 2; i++)
    {
        a = abs(x[2 * i]);
        b = abs(x[2 * i + 1]);
        x[i] = max(fixed_mul_8_8(ONE_OVER_SQRT_TWO, fixed_add_saturate_8_8(a, b)), max(a, b));
        // The "magic" multiplicative constant is greater than 1, so we have to use a trick: we instead do
        // x + (magic-1)x
        x[i] = fixed_add_saturate_8_8(x[i], fixed_mul_8_8(MODULUS_MAGIC, x[i]));
        /* Oh yeah, and also look for the maximum */
        /* Also skip first element since it's usually the loudest one.*/
        if (x[i] > maxi && i != 0)
        {
            maxi = x[i];
            i_maxi = i;
        }
    }
    /*
    float fa = (uint8_t) x[i_maxi-1];
    float fb = (uint8_t) x[i_maxi];
    float fc = (uint8_t) x[i_maxi + 1];
    float f_step = (fa*((float)i_maxi-1)+fb*(float)i_maxi+fc*((float)i_maxi+1))/(fa+fb+fc);
    */
    return i_maxi * frequency / size;
}

Fixed8FFT::Fixed8FFT(uint8_t input_pin, uint16_t sample_size, uint16_t frequency, fft_backend backend)
: FFT_backend_template( input_pin )
{
    /* Validate sample_size */
    if (get_power_of_two(sample_size) == 0)
    { 
        m_sample_size = 0;
        return;
    }

    m_sample_size = sample_size;
    if (!allocate_data_array())
    {
        m_sample_size = 0;
        return;
    }

    cli();
    interrupt_data.data = reinterpret_cast<int8_t *>(m_data);
    interrupt_data.array_pos = 0;
    interrupt_data.adc_pin = input_pin;
    interrupt_data.offset_x = 70;
    interrupt_data.scale_x = 4;
    interrupt_data.array_size = get_power_of_two(sample_size);
    sei();
    return;
}

bool Fixed8FFT::allocate_data_array()
{
    m_data = calloc(m_sample_size, sizeof(fixed8_t));

    if (m_data != nullptr)
        return 1;

    ERROR(F("Fixed8FFT: Failed to allocate data array. Size: "), sizeof(fixed8_t) * m_sample_size, F(" bytes"));
    return 0;
}

void Fixed8FFT::deallocate_data_array()
{
    if (m_data == nullptr)
        return;

    free(m_data);
    m_data = nullptr;
    return;
}

bool Fixed8FFT::set_sample_size(uint16_t sample_size)
{
    if (m_sample_size == sample_size)
        return 1;

    if (get_power_of_two(sample_size) == 0)
        return 1;

    m_sample_size = sample_size;

    cli();
    deallocate_data_array();

    if (!allocate_data_array())
    {
        interrupt_data.array_size = 0;
        interrupt_data.array_pos = 0;
        m_sample_size = 0;
        return 1;
    }

    interrupt_data.array_size = get_power_of_two(sample_size);
    interrupt_data.array_pos = 0;

    sei();
    return 0;
}

uint16_t Fixed8FFT::calculate()
{
    if (interrupt_data.array_pos == 1 << interrupt_data.array_size)
    {
        uint16_t temp = 0;

        if (millis() - last_result_time < 250)
        {
            calculate_scaling();
            last_result_time = millis();
        }

        fft(interrupt_data.data, m_sample_size);

        temp = modulus(interrupt_data.data, m_sample_size, m_sampling_frequency);
        cli();
        interrupt_data.array_pos = 0;
        sei();
        return temp;
    }
    return 0;
}

__attribute__((signal)) void __vector_timer1_compb_adc_read_byte()
{
    adc_sample_interrupt *data = (struct adc_sample_interrupt*) get_isr_data_ptr(TIMER1_COMPB_ptr);

    /* Check if data array is filled with data */
    if (data->array_pos >= 1 << data->array_size)
    {
        return;
    }

    ADMUX = (1 << 6) | (data->adc_pin & 0x15);

    /* Start conversion */
    _SFR_BYTE(ADCSRA) |= _BV(ADSC);

    /* Adc is cleared when conversion finishes */
    while (bit_is_set(ADCSRA, ADSC))
        ;
    /* Calculate the scaling values */
    uint16_t min_val = constrain(data->offset_x * 8 - data->scale_x * 32, 0, 1024);
    uint16_t max_val = constrain(data->offset_x * 8 + data->scale_x * 32, 0, 1024);

    /* Scale the adc reading to best fit in uint8_t. Then save it */
    data->data[data->array_pos] = map(constrain(ADC, min_val, max_val), min_val, max_val, -128, 127);
    data->array_pos += 1;
    return;
}


/* 
 * Internal definitions for calculate_scaling() 
 * 
 * @Fixed8FFT_min_dynamic_range: 
 * 
 * @Fixed8FFT_optimal_dynamic_range8bit: 
*/
#define Fixed8FFT_min_dynamic_range 200
#define Fixed8FFT_optimal_dynamic_range8bit 230

void Fixed8FFT::calculate_scaling()
{
    if (interrupt_data.array_pos != 1 << interrupt_data.array_size)
        return;

    int16_t highest = -128;
    int16_t lowest = 127;
    int32_t average = 0;
    int16_t used_dynamic_range = 0;
    //int16_t real_dynamic_range = 0;

    //int16_t real_highest = 0;
    //int16_t real_lowest = 0;
    cli();

    /* Calculate avarage, highest & lowest values */
    for (uint16_t i = 0; i < m_sample_size; i++)
    {
        average += reinterpret_cast<int8_t *>(m_data)[i];

        if (reinterpret_cast<int8_t *>(m_data)[i] > highest)
            highest = reinterpret_cast<int8_t *>(m_data)[i];

        if (reinterpret_cast<int8_t *>(m_data)[i] < lowest)
            lowest = reinterpret_cast<int8_t *>(m_data)[i];
    }
    
    used_dynamic_range = highest - lowest;

    /*
    real_lowest = map(lowest, -128, 127,
                      (interrupt_data.offset_x * 8 - interrupt_data.scale_x * 32),
                      (interrupt_data.offset_x * 8 + interrupt_data.scale_x * 32));

    real_highest = map(highest, -128, 127,
                       (interrupt_data.offset_x * 8 - interrupt_data.scale_x * 32),
                       (interrupt_data.offset_x * 8 + interrupt_data.scale_x * 32));

    real_dynamic_range = real_highest - real_lowest;
    */

    /* Check if we can use offset to get better signal range */
    if (highest == 127 || lowest == -128)
    {
        /* Calculate optimal value for offset_x. */
        /* aka try to place offset_x in the middle of the signal */
        average /= m_sample_size;
        // INFO(F("Average: "), average);

        /* Convert average from -128-127 to original adc reading */
        average = map(average, -128, 127,
                      (interrupt_data.offset_x * 8 - interrupt_data.scale_x * 32),
                      (interrupt_data.offset_x * 8 + interrupt_data.scale_x * 32));

        // INFO(F("Real average: "), average);
        /* Calculate offset */
        average = average - interrupt_data.offset_x * 8;
        // INFO(F("Offset fix: "), average);

        average = interrupt_data.offset_x + average;
        // INFO(F("New average: "), average);

        /* Check if new range is under 0*/
        if (map(-128, -128, 127,
                (average * 8 - interrupt_data.scale_x * 32),
                (average * 8 + interrupt_data.scale_x * 32)) < 0)
            goto skip_offset;

        /* Check if new range is over 1024*/
        if (map(127, -128, 127,
                (average * 8 - interrupt_data.scale_x * 32),
                (average * 8 + interrupt_data.scale_x * 32)) > 1024)
            goto skip_offset;

        interrupt_data.offset_x = constrain(average, 0, 128);
        // DEBUG(F("New offset: "), (uint16_t) interrupt_data.offset_x);
    }
skip_offset:

    if (used_dynamic_range > 230)
    {
        interrupt_data.scale_x = constrain(interrupt_data.scale_x + 1, 0, 16);
        // DEBUG(F("SCALE_X: "), (uint8_t) interrupt_data.scale_x);
    }
    else if (used_dynamic_range < Fixed8FFT_optimal_dynamic_range8bit)
    {
        interrupt_data.scale_x = constrain(interrupt_data.scale_x - 1, 5, 16);
        // DEBUG(F("SCALE_X: "), (uint8_t) interrupt_data.scale_x);
    }
    sei();
    return;
}


/**
 * @brief 2^n Returns the n if the number is power of two
 * 
 * @note Remember to disable interrupts before calling this function
 * @param value 
 * @return uint8_t 
 */
uint8_t Fixed8FFT::get_power_of_two(uint16_t value)
{
    /* Check if number is not power of 2 */
    if (value != 0 && (value & (value - 1)) != 0)
    {
        ERROR(F("approxFFT sample size is not power of 2. Size"), value);
        return 0;
    }

    /* Get sample size as the power of 2^n */
    for (uint16_t i = 0; i < 16; i++)
    {
        if (value >> i == 1) return i;
    }

    /* Idk if we get here */
    return 0;
}

vector_t Fixed8FFT::get_read_vector()
{
    return __vector_timer1_compb_adc_read_byte;
}

/* Possible mem corruption. If Fixed8FFT gets destroyed before
     &interrupt_data gets removed from the isr_vector_data_pointer_table */
void *Fixed8FFT::get_read_vector_data_pointer()
{
    return (void*) &interrupt_data;
}

Fixed8FFT::~Fixed8FFT()
{
    deallocate_data_array();
    return;
}