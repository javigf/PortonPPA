/*
 * EEPROM Manipulation File
 * ADDR = 0
 * ADDR = 1
 * ADDR = 2
 * ADDR = 3 RF Control count
 */

#define CTRL1 1
#define CTRL2 2


bool system_configured = 0;
unsigned int memory_position = 0;

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.

int EEPROMWritelong (unsigned int address, long value) {
        //Decomposition from a long to 4 bytes by using bitshift.
        //One = Most significant -> Four = Least significant byte
        byte four = (value & 0xFF);
        byte three = ((value >> 8) & 0xFF);
        byte two = ((value >> 16) & 0xFF);
        byte one = ((value >> 24) & 0xFF);

        //Write the 4 bytes into the eeprom memory.
        EEPROM.write(address, four);
        EEPROM.write(address + 1, three);
        EEPROM.write(address + 2, two);
        EEPROM.write(address + 3, one);

        return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
unsigned long EEPROMReadlong ( unsigned long address) {
        //Read the 4 bytes from the eeprom memory.
        long four = EEPROM.read(address);
        long three = EEPROM.read(address + 1);
        long two = EEPROM.read(address + 2);
        long one = EEPROM.read(address + 3);

        //Return the recomposed long by using bitshift.
        return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controls Management
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EEPROM pos = 0
// 0xAA MEANS EEPROM initialized
// TODO REVIEW POSITION IS NOT CORRECT -> DONE ON PURPOSE FOR TESTING
int first_time_run (void){
        unsigned int EEPROM_end_address = 0;

        EEPROM_end_address = EEPROM.length();
        if ((EEPROM.read(0)) != 0xAA) {
                for (unsigned int i = 0; i < EEPROM_end_address; i++)
                        EEPROM.write(i, 0);
                EEPROM.write((EEPROM_end_address - 1), 0xAA);
                Serial.println ("EEPROM Clear");
        }
        return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EEPROM pos = 2
// Check for system configured
int check_configuration (void){
        first_time_run();
        if ((EEPROM.read (2) == 1))
                system_configured = 1;

        return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EEPROM pos = 3
// Check Controls saved
unsigned int count_controls_in_EEPROM (void){
        unsigned int control_count = 0;

        control_count = EEPROM.read (3);

        return control_count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check Controls saved
// EEPROM pos = 3
unsigned int add_controls_to_EEPROM (void){
        unsigned int control_count = 0;

        control_count = count_controls_in_EEPROM ();
        EEPROM.write (3,control_count++);

        return control_count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controls should be added in memory address multiplo of 4/8/12/16/20 etc
bool check_existing_control (unsigned long rf_addr){
        bool status = 0;
        //unsigned int i = 0, controls;
        
        /*controls = count_controls_in_EEPROM ();                
        if (system_configured && ((controls > 0) && (controls <= MAX_CONTROLS))) {
                
            do {
                if (rf_addr == EEPROMReadlong (i*4))
                    status = 1;
                ++i;
                } while (!status && (i <= MAX_CONTROLS));
        }*/

        if ((rf_addr == CTRL1) || (rf_addr == CTRL2))
            status = 1;

        return status;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controls should be added in memory address multiplo of 4/8/12/16/20 etc
int add_control (unsigned long rf_addr){
        unsigned int controls = 0;
        
        if (system_configured && (!(check_existing_control (0)))) { //TODO VER 
                controls = count_controls_in_EEPROM ();

                if (controls < MAX_CONTROLS) {
                    EEPROMWritelong ((4 * (controls++)), rf_addr);
                    add_controls_to_EEPROM ();
                }
        }

        return 0;
}
