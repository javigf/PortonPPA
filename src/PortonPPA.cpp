// https://www.youtube.com/watch?v=ZywX3YaGp2I
// http://www.ppa.com.br/CATALOGO%20DE%20PRODUCTOS%20PPA_2014_ESPANOL.pdf
// http://www.seguridadactual.com.ar/manuales/manualfacilitynewespanol.pdf
/*Configuraciones estándar
· Fuerza = Máximo.
· Cierre = Semiautomático.
· Tiempo de Apertura y Cierre: 120 segundos.
· Tiempo de Freno (apertura / cierre): 500 milisegundos.
· Sistema de fin de carrera: Digital.
· Límite de fin de carrera = trayecto / 16.
· Retirada – Fin de recurrido de apertura: 2 pulsos.
· Retirada – Fin de recurrido de cierre: 0 pulso.
· Fuerza del torque pulsante (apertura) = nivel 12.
· Fuerza del torque pulsante (cierre) = nivel 7.
· Fuerza del torque pulsante de adquisición (apertura) = nivel 16.
· Fuerza del torque pulsante de adquisición (cierre) = nivel 10.
· Recurrido: 0.
Para volver las configuraciones estándar:
1. Cierre el jumper PROG. El LED rojo SN queda encendido con baja intensidad.
2. Apriete y suelte, simultáneamente, los dos botones de controle remoto (grabado) dos
veces, pausadamente; el LED parpadea. Aguarde 5 segundos.
3. Tras volver las configuraciones estándar, el LED SN parpadeará 4 veces.
4. Abra el jumper PROG.
Nota: Tras la programación del tópico Configuraciones estándar de fábrica, es
necesario memorizar nuevamente el recurrido; de lo contrario, la unidad de control no irá
funcionar (Sistema de fin de carrera digital).
*/
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
        //If a valid data is received, print ADDRESS CODE and Buttons values
        if (ACT_HT6P20B_RX (rfControl_1)) {
                addr = rfControl_1.addr;
            //NORMAL OPEATION MODE- JUMPER OFF
            if (!programming_mode) {    
                // CHECK FOR EXISTING VALID CONTROL ADDRESS IN MEMORY - eepr.h
                if (check_existing_control (addr)){
                    if (!motor_status){
                        if (rfControl_1.btn1 && !rfControl_1.btn2){
                            //move_motor_forward ();
                            motor_status = 1;
                            motor_direction = 1;
                        }
                        else if (!rfControl_1.btn1 && rfControl_1.btn2){
                            //move_motor_backward ();
                            motor_status = 1;
                            motor_direction = 0;   
                        }
                        //else if (rfControl_1.btn1 && rfControl_1.btn2)
                            // enter in programming mode depending on how many buttons pressed
                    }
                    else
                        motor_status = 0;
                }
            }
            //PROGRAMMING MODE ACTIVE - JUMPER ON
            else 
                // BUTTON IS BEING PRESSED
                if (!programRF)
                    add_control (addr);
        }

       
        if (motor_status && motor_direction)
            move_motor_forward ();
        else if (motor_status && !motor_direction)
            move_motor_backward ();
        else if (!motor_status)
            stop_motor ();
        // here is where you'd put code that needs to be running all the time.

        // check to see if it's time to blink the LED; that is, if the
        // difference between the current time and last time you blinked
        // the LED is bigger than the interval at which you want to
        // blink the LED.
        unsigned long currentMillis = millis();
        
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;

            //if 
            
        }
         blink ();

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
        
        

        //encoder_status();
        //blink ();
//#ifdef DEBUG
        //pinMode(led, INPUT);
        //int led_status = analogRead(0);
        //Serial.print("PGM Status: "); Serial.println (led_status);
        //delay(100);
//#endif
}
