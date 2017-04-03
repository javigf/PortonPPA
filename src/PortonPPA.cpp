#define DEBUG

#include <Arduino.h>
#include "PortonPPA.h"


void setup() {
        Serial.begin(115200);
        init_pins();
        check_configuration ();
        //test_motor();
}

void loop() {

        check_programming_mode ();
        if(ACT_HT6P20B_RX (rfControl_1)) {
                //If a valid data is received, print ADDRESS CODE and Buttons values

#ifdef DEBUG
                Serial.print("Address: "); Serial.println(rfControl_1.addr,HEX);
                Serial.print("Button1: "); Serial.println(rfControl_1.btn1,BIN);
                Serial.print("Button2: "); Serial.println(rfControl_1.btn2,BIN);
                Serial.println();

                EEPROMWritelong (memory_position,rfControl_1.addr);
                Serial.print("Address: "); Serial.println(EEPROMReadlong(memory_position),HEX);
                Serial.print("Mem pos: "); Serial.println(memory_position);
#endif


        }

        long newPosition = myEnc.read();
        //Serial.println(newPosition);
        if (newPosition != oldPosition) {
                oldPosition = newPosition;
                Serial.println(newPosition);
        }

        move_motor_forward(-10);
        //move_motor_backward(10);
        delay(10000);

        //encoder_status();
        //blink ();
//#ifdef DEBUG
        //pinMode(led, INPUT);
        //int led_status = analogRead(0);
        //Serial.print("PGM Status: "); Serial.println (led_status);
        //delay(100);
//#endif
}
