#include <Encoder.h>


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached

long newPosition, oldPosition = -999;

long encoder_status (void) {

        newPosition = myEnc.read();

        if (newPosition != oldPosition) {
                oldPosition = newPosition;
#ifdef DEBUG                
                Serial.println(newPosition);
#endif
                return newPosition;
        }

        return 0;
}
