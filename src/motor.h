#define triac 5
#define rele1 6
#define rele2 7

int motor_max_position = 0;
int motor_min_position = 0;

// TODO SAVE VALUES IN EEPROM

int activate_triac (void){
    digitalWrite (triac, HIGH);
    delayMicroseconds (100);
    digitalWrite (triac, LOW);
    delayMicroseconds (100);

  return 0;
}

int stop_motor (void){
        digitalWrite (triac, LOW);
        digitalWrite (rele1, LOW);
        digitalWrite (rele2, LOW);

        return 0;
}

int move_motor_forward (void){
      activate_triac ();
        digitalWrite (rele1, LOW);
        digitalWrite (rele2, HIGH);

        return 0;
}

int move_motor_backward (void){
        activate_triac ();
        digitalWrite (rele1, HIGH);
        digitalWrite (rele2, LOW);

        return 0;
}

int move_motor_forward (long position){
        delay (1000);
        long enc_value = myEnc.read ();
        while (position < enc_value){
                move_motor_forward ();
                enc_value = myEnc.read ();
                Serial.println (enc_value);
        }
        stop_motor ();

        return 0;
}

int move_motor_backward (long position){
        delay (1000);
        while (position < myEnc.read())
                move_motor_backward();
        return 0;
}



// 0 : STOP
// 1 : FORWARD
// 2 : BACKWARD

int drive_motor (int a){

        if (a == 0)
                stop_motor ();
        else if (a == 1)
                move_motor_forward();
        else if (a == 2)
                move_motor_backward();
        return 0;
}

int save_motor_position (void){


        return 0;
}

int test_motor (void){

        drive_motor(1);
        delay (300);
        drive_motor(0);
        delay (1000);
        drive_motor(2);
        delay (300);
        drive_motor(0);
        delay (10);

        return 0;
}
