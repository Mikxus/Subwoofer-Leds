/*
 * MIT License
 *
 * Copyright (c) 2022 Mikko Heinänen
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
#ifndef _RISR_H_
#define _RISR_H_

#include <stddef.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief void function pointer to interrupt vector
     *
     */
    typedef void (*vector_t)(void);

    /**
     * @brief ISR for bad interrupts
     *
     * @return vector_t
     */
    extern void runtime_bad_isr(void);

    /**
     * @brief an array of function pointers that hold the adresses of dynamic ISRs
     *
     */
    extern void (*isr_vector_table[])(void);
    extern void *isr_vector_data_pointer_table[];

#if ENABLE_ISR_VECTOR_TABLE_SIZE == 1
    extern const uint8_t isr_vector_table_size;
#endif

#if ENABLE_ISR_VECTOR_DATA_TABLE_SIZE == 1
    extern const uint8_t isr_vector_data_table_size;
#endif

    /**
     * @brief  List of interrupt vectors.
     * @note
     */
    typedef enum
    {

#if defined(INT0_used) || defined(INT0_unsafe)
        INT0_,
#define enum_used
#endif

#if defined(INT1_used) || defined(INT1_unsafe)
        INT1_,
#define enum_used
#endif

#if defined(PCINT0_used) || defined(PCINT0_unsafe)
        PCINT0_,
#define enum_used
#endif

#if defined(PCINT1_used) || defined(PCINT1_unsafe)
        PCINT1_,
#define enum_used
#endif

#if defined(PCINT2_used) || defined(PCINT2_unsafe)
        PCINT2_,
#define enum_used
#endif

#if defined(WDT_used) || defined(WDT_unsafe)
        WDT_,
#define enum_used
#endif

#if defined(TIMER2_COMPA_used) || defined(TIMER2_COMPA_unsafe)
        TIMER2_COMPA_,
#define enum_used
#endif

#if defined(TIMER2_COMPB_used) || defined(TIMER2_COMPB_unsafe)
        TIMER2_COMPB_,
#define enum_used
#endif

#if defined(TIMER2_OVF_used) || defined(TIMER2_OVF_unsafe)
        TIMER2_OVF_,
#define enum_used
#endif

#if defined(TIMER1_CAPT_used) || defined(TIMER1_CAPT_unsafe)
        TIMER1_CAPT_,
#define enum_used
#endif

#if defined(TIMER1_COMPA_used) || defined(TIMER1_COMPA_unsafe)
        TIMER1_COMPA_,
#define enum_used
#endif
#if defined(TIMER1_COMPB_used) || defined(TIMER1_COMPB_unsafe)
        TIMER1_COMPB_,
#define enum_used
#endif

#if defined(TIMER1_OVF_used) || defined(TIMER1_OVF_unsafe)
        TIMER1_OVF_,
#define enum_used
#endif

#if defined(TIMER0_COMPA_used) || defined(TIMER0_COMPA_unsafe)
        TIMER0_COMPA_,
#define enum_used
#endif

#if defined(TIMER0_COMPB_used) || defined(TIMER0_COMPB_unsafe)
        TIMER0_COMPB_,
#define enum_used
#endif

#if defined(TIMER0_OVF_used) || defined(TIMER0_OVF_unsafe)
        TIMER0_OVF_,
#define enum_used
#endif

#if defined(SPI_STC_used) || defined(SPI_STC_unsafe)
        SPI_STC_,
#define enum_used
#endif

#if defined(USART_RX_used) || defined(USART_RX_unsafe)
        USART_RX_,
#define enum_used
#endif

#if defined(USART_UDRE_used) || defined(USART_UDRE_unsafe)
        USART_UDRE_,
#define enum_used
#endif

#if defined(USART_TX_used) || defined(USART_TX_unsafe)
        USART_TX_,
#define enum_used
#endif

#if defined(ADC_used) || defined(ADC_unsafe)
        ADC_,
#define enum_used
#endif

#if defined(EE_READY_used) || defined(EE_READY_unsafe)
        EE_READY_,
#define enum_used
#endif

#if defined(ANALOG_COMP_used) || defined(ANALOG_COMP_unsafe)
        ANALOG_COMP_,
#define enum_used
#endif

#if defined(TWI_used) || defined(TWI_unsafe)
        TWI_,
#define enum_used
#endif

#if defined(SPM_READY_used) || defined(SPM_READY_unsafe)
        SPM_READY_,
#define enum_used
#endif
    } isr_vectors;

#ifndef enum_used
#error rISR: No interrupt vectors defined.
#endif
#undef enum_used

    /**
     * @brief  Binds function pointer to selected isr
     * @note
     * @param  isr_name:       Name of the isr
     * @param  isr_func_ptr:   Function pointer to be binded
     * @retval None
     */
    extern void bind_isr(isr_vectors isr_name, vector_t isr_func_ptr);

    /**
     * @brief  Unbinds the selected isr vector
     * @note
     * @param  isr_name:
     * @retval None
     */
    extern void unbind_isr(isr_vectors isr_name);

    /**
     * @brief Binds pointer to data thingy
     *
     * @param isr_name
     * @param pointer
     */
    extern void bind_isr_data_ptr(isr_vectors isr_name, void *pointer);

    extern void unbind_isr_data_ptr(isr_vectors isr_name);

    /**
     * @brief Returns pointer to isr if the vector is used. Otherwise null.
     *
     * @param isr_name
     * @return vector_t
     */
    extern vector_t get_isr_vector(isr_vectors isr_name);

    extern void *get_isr_data_ptr(isr_vectors isr_name);

#if defined(TIMER1_COMPB_struct)
    extern volatile struct adc_sample_interrupt TIMER1_COMPB_data;
#endif

#ifdef __cplusplus
};
#endif

#endif