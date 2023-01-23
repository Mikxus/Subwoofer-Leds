#include "interrupt.h"

__attribute__ ((signal)) void __vector_timer1_compb_adc_read_byte()
{
    if ( TIMER1_COMPB_data.array_pos >= 1 << TIMER1_COMPB_data.array_size ) return; 

    ADMUX = (1 << 6) | (TIMER1_COMPB_data.adc_pin & 0x15);

    /* Start conversion */
    _SFR_BYTE(ADCSRA) |= _BV(ADSC);

    /* Adc is cleared when conversion finishes */
    while (bit_is_set(ADCSRA, ADSC));

    uint16_t min_val = (TIMER1_COMPB_data.offset_x * 8 - TIMER1_COMPB_data.scale_x * 32) & 0x3FF;
    uint16_t max_val = (TIMER1_COMPB_data.offset_x * 8 + TIMER1_COMPB_data.scale_x * 32) & 0x3FF; 
    
    /* Scale the adc reading to best fit in uint8_t. Then save it */
    TIMER1_COMPB_data.data[1 << TIMER1_COMPB_data.array_pos] = map(constrain(ADC, min_val, max_val), min_val, max_val, -128, 127);    

    TIMER1_COMPB_data.array_pos += 1;

    return;
}

interrupt_manager interrupt;