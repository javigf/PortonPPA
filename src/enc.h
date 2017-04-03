#include <Encoder.h>


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached

long oldPosition  = -999;

long encoder_status (void) {

        static long newPosition = myEnc.read();
        //Serial.println(newPosition);
        if (newPosition != oldPosition) {
                oldPosition = newPosition;
                Serial.println(newPosition);
                return newPosition;
        }

        return 0;
}
