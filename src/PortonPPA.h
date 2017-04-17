#include <EEPROM.h>
#include "RF.h"
#include "enc.h"
#include "eepr.h"
#include "motor.h"

#define INTERVAL_VALUE 200

unsigned int blink_times = 5;
bool check_pgm_enable = 0;

// Variables will change :
//bool ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
unsigned long interval = 100;           // interval at which to blink (milliseconds)
unsigned long interval_blink = INTERVAL_VALUE;

// WHEN PROGRAMMING MODE & RF CONTROL ADDED BLINK 2 TIMES
bool programming_mode = 0;

// WHEN NO RF CONTROL BLINK 3 TIMES
unsigned int led_blink_count = 0;

#define led A0

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
    unsigned int value;
        
    if (check_pgm_enable){
        pinMode (led,INPUT);
        value = analogRead (led);
            
        if  (value > 100)
            programming_mode = 1;
        else
            programming_mode = 0;
        }      

    Serial.println (value);
    return 0;
}

int blink (void){
        
static unsigned long last_blink_time = 0;

if(millis() - last_blink_time >= interval_blink) {

        led_blink_count++;
        
        if (led_blink_count > blink_times){
            led_blink_count = 0;
            interval_blink = INTERVAL_VALUE * 10;
            check_pgm_enable = 1;
        }
        else{
            interval_blink = INTERVAL_VALUE;
            pinMode (led,OUTPUT);
            check_pgm_enable = 0;
            int state = digitalRead (led);
            digitalWrite (led,!state);
        }

    last_blink_time += interval_blink ; 
}
        
        return 0;
}


