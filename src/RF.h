#define MAX_CONTROLS    10
const int pinRF         = 4;  // Pin where RF Module is connected. If necessary, change this for your project
const int programRF     = 9;

int lambda;      // on pulse clock width (if fosc = 2KHz than lambda = 500 us)

struct rfControl {        //Struct for RF Remote Controls

        unsigned long addr ; //ADDRESS CODE
        bool btn1;   //BUTTON 1
        bool btn2;   //BUTTON 2
        bool btn3;   //BUTTON 3 > NOT ALL CONTROLS HAS 3 BUTTONS 
};

struct rfControl rfControl_1;    //Set variable rfControl_1 as rfControl type


bool ACT_HT6P20B_RX(struct rfControl &_rfControl){

        static bool startbit; //checks if start bit was identified
        static int counter;     //received bits counter: 22 of Address + 2 of Data + 4 of EndCode (Anti-Code)
        static unsigned long buffer; //buffer for received data storage

        int dur0, dur1; // pulses durations (auxiliary)

        if (!startbit) {// Check the PILOT CODE until START BIT;
                dur0 = pulseIn(pinRF, LOW); //Check how long DOUT was "0" (ZERO) (refers to PILOT CODE)

                //If time at "0" is between 9200 us (23 cycles of 400us) and 13800 us (23 cycles of 600 us).
                if((dur0 > 9200) && (dur0 < 13800) && !startbit) {
                        //calculate wave length - lambda
                        lambda = dur0 / 23;

                        //Reset variables
                        dur0 = 0;
                        buffer = 0;
                        counter = 0;

                        startbit = true;
                }
        }

        //If Start Bit is OK, then starts measure os how long the signal is level "1" and check is value is into acceptable range.
        if (startbit && counter < 28)  {
                ++counter;

                dur1 = pulseIn(pinRF, HIGH);

                if((dur1 > 0.5 * lambda) && (dur1 < (1.5 * lambda))) { //If pulse width at "1" is between "0.5 and 1.5 lambda", means that pulse is only one lambda, so the data é "1".

                        buffer = (buffer << 1) + 1; // add "1" on data buffer
                }
                else if((dur1 > 1.5 * lambda) && (dur1 < (2.5 * lambda))) {//If pulse width at "1" is between "1.5 and 2.5 lambda", means that pulse is two lambdas, so the data é "0".

                        buffer = (buffer << 1); // add "0" on data buffer
                }
                else  {
                        //Reset the loop
                        startbit = false;
                }
        }

        //Check if all 28 bits were received (22 of Address + 2 of Data + 4 of Anti-Code)
        if (counter==28) {
                // Check if Anti-Code is OK (last 4 bits of buffer equal "0101")
                if ((bitRead(buffer, 0) == 1) && (bitRead(buffer, 1) == 0) && (bitRead(buffer, 2) == 1) && (bitRead(buffer, 3) == 0)) {
                        counter = 0;
                        startbit = false;

                        //Get ADDRESS CODE from Buffer
                        _rfControl.addr = buffer >> 6;

                        //Get Buttons from Buffer
                        _rfControl.btn1 = bitRead(buffer,4);
                        _rfControl.btn2 = bitRead(buffer,5);

                        //Serial.print("Address: "); Serial.println(_rfControl.addr, HEX);
                        //Serial.print("Button1: "); Serial.println(_rfControl.btn1, BIN);
                        //Serial.print("Button2: "); Serial.println(_rfControl.btn2, BIN);
                        //Serial.println();

                        //If a valid data is received, return OK
                        return true;
                }
                else {
                        //Reset the loop
                        startbit = false;
                }
        }

        //If none valid data is received, return NULL and FALSE values
        _rfControl.addr = NULL;
        _rfControl.btn1 = NULL;
        _rfControl.btn2 = NULL;

        return false;
}
