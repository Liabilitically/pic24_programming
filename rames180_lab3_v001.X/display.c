/*
 * File:   display.c
 * Author: tarak
 *
 * Created on March 2, 2026, 1:13 AM
 */


#include "xc.h"
#include "display.h"

void display_init(void) {
    AD1PCFG = 0xFFFF;
    TRISB &= 0b1111000000000011;
    LATBbits.LATB10 = 0;
    LATBbits.LATB11 = 0;
    LATB |= 0b0000001111111100;
}

void display_showChar(char myChar, int myDigit) {
    LATB |= 0b0000001111111100;

    LATBbits.LATB10 = ~myDigit;
    LATBbits.LATB11 = myDigit;

    if (myChar == '0') {
        LATB &= 0b1111110000001100;
    } else if (myChar == '1') {
        LATB &= 0b1111111001111100;
    } else if (myChar == '2') {
        LATB &= 0b1111110010010100;
    } else if (myChar == '3') {
        LATB &= 0b1111110000110100;
    } else if (myChar == '4') {
        LATB &= 0b1111111001100100;
    } else if (myChar == '5') {
        LATB &= 0b1111110100100100;
    } else if (myChar == '6') {
        LATB &= 0b1111110100000100;
    } else if (myChar == '7') {
        LATB &= 0b1111110001111100;
    } else if (myChar == '8') {
        LATB &= 0b1111110000000100;
    } else if (myChar == '9') {
        LATB &= 0b1111110000100100;
    } else if (myChar == 'A') {
        LATB &= 0b1111110001000100;
    } else if (myChar == 'b') {
        LATB &= 0b1111111100000100;
    } else if (myChar == 'C') {
        LATB &= 0b1111110110001100;
    } else if (myChar == 'd') {
        LATB &= 0b1111111000010100;
    } else if (myChar == 'E') {
        LATB &= 0b1111110110000100;
    } else if (myChar == 'F') {
        LATB &= 0b1111110111000100;
    }
}
