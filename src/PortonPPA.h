

#include <EEPROM.h>
#include "RF.h"
#include "enc.h"
#include "eepr.h"
#include "motor.h"

bool programming_mode = 0;

#define led 8

int init_pins (void){

        pinMode(pinRF, INPUT);
        pinMode(led, OUTPUT);
        pinMode(triac, OUTPUT);
        pinMode(rele1, OUTPUT);
        pinMode(rele2, OUTPUT);
        pinMode(programRF, INPUT);

        stop_motor();

        return 0;
}

int check_programming_mode (void){
        static unsigned int counter = 0;
        unsigned int val = 0;
        pinMode(led,INPUT);

        val = analogRead(led);

        if (val > 100)
                counter++;

        if ((counter > 50) && (val > 100)) {
                programming_mode = 1;
                if (counter > 0xFFF0)
                        counter = 50;
        }
        else{
                programming_mode = 0;
                counter = 0;
        }

        return 0;
}

int blink (void){
        int state = digitalRead (led);
        digitalWrite (led,!state);
        delay (300);

        return 0;
}
