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
        Serial.print(F("   |"));
    }
    Serial.println();
    
    effect.SetFrequency(1400);
    printMultiple(' ',4,0);
    uint32_t timeMicros;
    for (uint8_t i = 0; i < 15; i++)
    {
        Serial.print(F("| "));
        if ( effect.SetSampleSize( pow(2,i) ) )
            {
                timeMicros = micros();

                for (uint16_t a = 0; a < 2000; a++)
                {
                    effect.Benchmark();
                }
                float time = micros() - timeMicros;
                if (time > 1000)
                {
                    time /= 1000;
                    Serial.print(time,2);
                    Serial.print(F("ms"));
                } else 
                {
                    Serial.print(time);
                    Serial.print(F("μs"));

                }

            } else
            {
                Serial.print(F("X"));
            }
        Serial.print(F(" |"));
    }
    Serial.println();
    Serial.println("Benchmark Completed");
    while(1)
    {
        delay(400);
    }
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
