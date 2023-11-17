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
#include "rISR.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Dynamic interrupt
 * @note
 *  
 *  Performance:
 *          Cycles: 19
 *              Before user interrupt: 11
 *              After user interrupt: 8
 *          Flash: 22 bytes
 *          Stack: 4 bytes
 * 
 *          This interrupt should compile in to the following asm:
 *          Tested on avr-gcc 12.2.0 using -O3 optimization
 * 
 *           push	r30
 *           push	r31
 *           lds	r30, &isr_vector_table + isr_vect * 2
 *           lds	r31, &isr_vector_table + isr_vect * 2 + 1
 *           icall
 *           pop	r31
 *           pop	r30
 *           reti
 */
#define dynamic_isr( isr_vect, isr_offset )         \
    ISR( isr_vect, ISR_NAKED )                      \
    {                                               \
        __asm__(                                    \
            "push r30   \n\t"                       \
            "push r31       "                       \
            :                                       \
            :                                       \
            : "memory");                            \
        register void (*user_isr)(void) asm("r30"); \
        user_isr =  isr_vector_table[ isr_offset];  \
        __asm__(                                    \
            "icall      \n\t"                       \
            "pop r31    \n\t"                       \
            "pop r30    \n\t"                       \
            "reti           "                       \
            :                                       \
            :  "r" (user_isr)                       \
            : "memory");                            \
    }                                   

/**
 * @brief   Fast dynamic interrupt.  
 * @note    Before returning from the interrupt you need to pop 31 and r30, 
 *          since they are pushed on the stack before your vector is executed. 
 *          The order to pop musn't change.
 * 
 *          This interrupt should compile into the following asm.
 *          Tested on avr-gcc 12.2.0 using -O3 optimization
 * 
 *          push r30
 *          push r31
 *          lds r30,  &isr_vector_table + isr_vect * 2
 *          lds r31,  &isr_vector_table + isr_vect * 2 + 1
 *          ijmp
 * 
 *  Performance:
 *          Cycles: 8
 *          Flash: 14 bytes
 *          Stack: 2 bytes
 */
#define unsafe_dynamic_isr( isr_vect, isr_offset )                  \
    ISR( isr_vect, ISR_NAKED )                                      \
    {                                                               \
        __asm__(                                                    \
        "push r30    \n\t"                                          \
        "push r31        "                                          \
        :                                                           \
        :                                                           \
        : "memory");                                                \
        volatile void (*ptr)(void) = isr_vector_table[ isr_offset ];\
        goto *ptr;                                                  \
    }

/* Dynamic interrupt routine for every isr */
#if defined(INT0_used)
        unsafe_dynamic_isr( INT0_vect, INT0_ );
    #elif defined(INT0_unsafe)
        unsafe_dynamic_isr( INT0_vect, INT0_ )
#endif

#if defined(INT1_used)
        dynamic_isr( INT1_vect, INT1_ )
    #elif defined(INT1_unsafe)
        unsafe_dynamic_isr( INT1_vect, INT1_ )
#endif

#if defined(PCINT0_used)
        dynamic_isr( PCINT0_vect, PCINT0_ )
    #elif defined(PCINT0_unsafe)
        unsafe_dynamic_isr( PCINT0_vect, PCINT0_ )
#endif

#if defined(PCINT1_used)
    dynamic_isr( PCINT1_vect, PCINT1_ )
    #elif defined(PCINT1_unsafe)
        unsafe_dynamic_isr( PCINT1_vect, PCINT1_ )
#endif

#if defined(PCINT2_used)
        dynamic_isr( PCINT2_vect, PCINT2_ )
    #elif defined(PCINT2_unsafe)
        unsafe_dynamic_isr( PCINT2_vect, PCINT2_ )
#endif

#if defined(WDT_used)
        dynamic_isr( WDT_vect, WDT_ )
    #elif defined(WDT_unsafe)
        unsafe_dynamic_isr( WDT_vect, WDT_ )
#endif

#if defined(TIMER2_COMPA_used)
        dynamic_isr( TIMER2_COMPA_vect, TIMER2_COMPA_ )
    #elif defined(TIMER2_COMPA_unsafe)
        unsafe_dynamic_isr( TIMER2_COMPA_vect, TIMER2_COMPA_ ) 
#endif

#if defined(TIMER2_COMPB_used)
        dynamic_isr( TIMER2_COMPB_vect, TIMER2_COMPB_ )
    #elif defined(TIMER2_COMPB_unsafe)
        unsafe_dynamic_isr( TIMER2_COMPB_vect, TIMER2_COMPB_ )
#endif

#if defined(TIMER2_OVF_used)
        dynamic_isr( TIMER2_OVF_vect, TIMER2_OVF_ )
    #elif defined(TIMER2_OVF_unsafe)
        unsafe_dynamic_isr( TIMER2_COMPB_vect, TIMER2_COMPB_ )
#endif

#if defined(TIMER1_CAPT_used)
        dynamic_isr( TIMER1_CAPT_vect, TIMER1_CAPT_)
    #elif defined(TIMER1_CAPT_unsafe)
        unsafe_dynamic_isr( TIMER1_CAPT_vect, TIMER1_CAPT_ )
#endif

#if defined(TIMER1_COMPA_used)
        dynamic_isr( TIMER1_COMPA_vect, TIMER1_COMPA_ )
    #elif defined(TIMER1_COMPA_unsafe)
        unsafe_dynamic_isr( TIMER1_COMPA_vect, TIMER1_COMPA_ )
#endif

#if defined(TIMER1_COMPB_used)
        dynamic_isr( TIMER1_COMPB_vect, TIMER1_COMPB_ )
    #elif defined(TIMER1_COMPB_unsafe)
        unsafe_dynamic_isr( TIMER1_COMPB_vect, TIMER1_COMPB_ )
#endif

#if defined(TIMER1_OVF_used)
        dynamic_isr( TIMER1_OVF_vect, TIMER1_OVF_)
    #elif defined(TIMER1_OVF_unsafe)
        unsafe_dynamic_isr( TIMER1_OVF_vect, TIMER1_OVF_ )
#endif

#if defined(TIMER0_COMPA_used)
        dynamic_isr( TIMER0_COMPA_vect, TIMER0_COMPA_ )
    #elif defined(TIMER0_COMPA_unsafe)
        unsafe_dynamic_isr( TIMER0_COMPA_vect, TIMER0_COMPA_ )
#endif

#if defined(TIMER0_COMPB_used)
        dynamic_isr( TIMER0_COMPB_vect, TIMER0_COMPB_ )
    #elif defined(TIMER0_COMPB_unsafe)
        unsafe_dynamic_isr( TIMER0_COMPB_vect, TIMER0_COMPB_ )
#endif

#if defined(TIMER0_OVF_used)
        dynamic_isr( TIMER0_OVF_vect, TIMER0_OVF_ )
    #elif defined(TIMER0_OVF_unsafe)
        unsafe_dynamic_isr( TIMER0_OVF_vect, TIMER0_OVF_ ) 
#endif

#if defined(SPI_STC_used)
        dynamic_isr( SPI_STC_vect, SPI_STC_ )
    #elif defined(SPI_STC_unsafe)
        unsafe_dynamic_isr( SPI_STC_vect, SPI_STC_ )
#endif

#if defined(USART_RX_used)
        dynamic_isr( USART_RX_vect, USART_RX_ )
    #elif defined(USART_RX_unsafe)
        unsafe_dynamic_isr( USART_RX_vect, USART_RX_ )
#endif

#if defined(USART_UDRE_used)
        dynamic_isr( USART_UDRE_vect, USART_UDRE_ )
    #elif defined(USART_UDRE_unsafe)
        unsafe_dynamic_isr( USART_UDRE_vect, USART_UDRE_ )
#endif

#if defined(USART_TX_used)
        dynamic_isr( USART_TX_vect, USART_TX_ )
    #elif defined(USART_TX_unsafe)
        unsafe_dynamic_isr( USART_TX_vect, USART_TX_ )
#endif

#if defined(ADC_used)
        dynamic_isr( ADC_vect, ADC_ )
    #elif defined(ADC_unsafe)
        unsafe_dynamic_isr( ADC_vect, ADC_ )
#endif

#if defined(EE_READY_used)
        dynamic_isr( EE_READY_vect, EE_READY_ )
    #elif defined(EE_READY_unsafe)
        unsafe_dynamic_isr( EE_READY_vect, EE_READY_ )
#endif

#if defined(ANALOG_COMP_used)
        dynamic_isr( ANALOG_COMP_vect, ANALOG_COMP_ )
    #elif defined(ANALOG_COMP_unsafe)
        unsafe_dynamic_isr( ANALOG_COMP_vect, ANALOG_COMP_)
#endif

#if defined(TWI_used)
        dynamic_isr( TWI_vect, TWI_ )
    #elif defined(TWI_unsafe)
        unsafe_dynamic_isr( TWI_vect, TWI_ )
#endif

#if defined(SPM_READY_used)
        dynamic_isr( SPM_READY_vect, SPM_READY_ )
    #elif defined(SPM_READY_unsafe)
        unsafe_dynamic_isr( SPM_READY_vect, SPM_READY_ )
#endif

__attribute__ ((signal)) void runtime_bad_isr( void )
{
    return;
}


void bind_isr( isr_vectors isr_name, vector_t vector )
{
    isr_vector_table[ isr_name ] = vector;
    return;
}

void unbind_isr( isr_vectors isr_name )
{
    isr_vector_table[ isr_name ] = runtime_bad_isr;
    return;
}

void bind_isr_data_ptr( isr_data_pointers isr_name, void *pointer )
{
    isr_vector_data_pointer_table[ isr_name ] = pointer;
    return;
}

void unbind_isr_data_ptr( isr_data_pointers isr_name )
{
    isr_vector_data_pointer_table[ isr_name ] = NULL;
    return;
}

vector_t get_isr_vector( isr_vectors isr_name )
{
    return isr_vector_table[ isr_name ];
}

void *get_isr_data_ptr( isr_data_pointers isr_name )
{
    return isr_vector_data_pointer_table[ isr_name ];
}

#ifdef __cplusplus
};
#endif
