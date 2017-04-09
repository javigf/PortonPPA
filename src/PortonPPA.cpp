// https://www.youtube.com/watch?v=ZywX3YaGp2I
// LED HIGH = ON
// LED BLINK 4 TIMES FORWARD - 3 TIMES BACKWARD

#define DEBUG

#include <Arduino.h>
#include "PortonPPA.h"

void setup() {
        Serial.begin(115200);
        init_pins();
        check_configuration ();
        //test_motor();
        Serial.println ("Starting");
        digitalWrite (led,HIGH);
        delay(500);
        digitalWrite (led,LOW);
}

void loop() {
    unsigned long addr;
        check_programming_mode ();          //LOCATED in PortonPPA.h
        if(ACT_HT6P20B_RX (rfControl_1)) {
            //If a valid data is received, print ADDRESS CODE and Buttons values
            //if (programming_mode && (!programRF)) {
            if (!programming_mode) {    //IF NORMAL OPEATION MODE
                addr = rfControl_1.addr;
                if (check_existing_control (addr))
                    if (!motor_status){
                        if (rfControl_1.btn1 && !rfControl_1.btn2)
                            move_motor_forward ();
                        else if (!rfControl_1.btn1 && rfControl_1.btn2)
                            move_motor_backward ();
                    }
                    else
                        stop_motor ();

            }
            else                

        }
                    
        long newPosition = myEnc.read();
        //Serial.println(newPosition);
        if (newPosition != oldPosition) {
                oldPosition = newPosition;
                Serial.println(newPosition);
        }

        /*
        #ifdef DEBUG
                    Serial.print("Address: "); Serial.println(rfControl_1.addr,HEX);
                    Serial.print("Button1: "); Serial.println(rfControl_1.btn1,BIN);
                    Serial.print("Button2: "); Serial.println(rfControl_1.btn2,BIN);
                    Serial.println();
    
                    //EEPROMWritelong (memory_position,rfControl_1.addr);
                    Serial.print("Address: "); Serial.println(EEPROMReadlong(memory_position),HEX);
                    Serial.print("Mem pos: "); Serial.println(memory_position);
                    
                    blink_times = 3;
        #endif
        */
                    
    

        //move_motor_forward(-10);
        //move_motor_backward(10);
        //delay(10000);
        
        // here is where you'd put code that needs to be running all the time.

        // check to see if it's time to blink the LED; that is, if the
        // difference between the current time and last time you blinked
        // the LED is bigger than the interval at which you want to
        // blink the LED.
        unsigned long currentMillis = millis();
        
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;
            blink ();
        }

        //encoder_status();
        //blink ();
//#ifdef DEBUG
        //pinMode(led, INPUT);
        //int led_status = analogRead(0);
        //Serial.print("PGM Status: "); Serial.println (led_status);
        //delay(100);
//#endif
}
