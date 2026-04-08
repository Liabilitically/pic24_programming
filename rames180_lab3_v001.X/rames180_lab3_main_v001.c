/*
 * File:   rames180_lab3_main_v001.c
 * Author: tarak
 *
 * Created on February 16, 2026, 2:01 AM
 */


#include "xc.h"
#include "display.h"
#include "keypad.h"
#include "rames180_lab3_asmLib_v001.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                        // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define LEFT 1
#define RIGHT 0

void delay(int delay_in_ms) {
    for (int num_ms = delay_in_ms; num_ms > 0; num_ms--)
        wait_tr_1ms();
}

int main(void) {
    display_init();
    keypad_init();

    char leftChar = ' ';
    char rightChar = ' ';

    while (1) {
        char keyPressed = keypad_getKey();

        if (keyPressed != ' ') {
            leftChar = rightChar;
            rightChar = keyPressed;
            delay(150);
        }

        display_showChar(leftChar, LEFT);
        delay(2);
        display_showChar(rightChar, RIGHT);
        delay(2);
    }
}
