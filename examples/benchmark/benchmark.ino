#include <Arduino.h>
#include </home/mikko/Documents/code/Subwoofer-Leds/scr/SubEffects.h>

SubEffects effect(0,6,100);         // effect( subwoofer analog pin , led data pin , led count )

void printMultiple();

void setup() {
    Serial.begin(115200);
}

void loop() {

    Serial.println(F("SubEffects"));
    printMultiple(' ',4,0);
    for (uint8_t i = 0; i < 8; i++)
    {
        Serial.print(F("| "));
        Serial.print( pow(2,i) );
        Serial.print(F(" |"));
    }
    Serial.println();
    
    effect.SetFrequency(1400);
    printMultiple(' ',4,0);
    bool enoughMem;
    uint32_t timeMicros;
    for (uint8_t i = 0; i < 8; i++)
    {
        timeMicros = micros();
        if ( effect.SetSampleSize( pow(2,i) ) )
            {
                enoughMem = true;
                for (uint16_t a = 0; a < 200; a++)
                {
                    effect.Update();
                }

            } else enoughMem = false;
        Serial.print(F("| "));
        if (enoughMem)
        {
            uint32_t time = micros() - timeMicros;
            Serial.print(time);
        } else
        {
            Serial.print(F("X"));
        }
        Serial.print(F(" |"));
    }
    Serial.println();
    Serial.println("Benchmark Completed");

    while(true);
}

void printMultiple(char a, uint16_t count, uint16_t modulo ) {
    for (uint16_t i = 0; i < count; i++)
    {
        if (modulo != 0)
        {
            if (i % modulo == 0) Serial.println();
        } else Serial.print(a);
    
    }
}
