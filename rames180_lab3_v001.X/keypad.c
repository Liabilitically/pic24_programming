/*
 * File:   keypad.c
 * Author: tarak
 *
 * Created on March 2, 2026, 1:15 AM
 */


#include "xc.h"
#include "keypad.h"

static unsigned int readKeyPadRAW(void);
static char mapKey(unsigned int rawKey);

void keypad_init(void){
    AD1PCFG |= 0xFFFF;
    TRISB &= 0b0000111111111111;
    TRISA |= 0b0000000000001111;

    _CN2PUE = 1;   // RA0
    _CN3PUE = 1;   // RA1
    _CN30PUE = 1;  // RA2
    _CN29PUE = 1;  // RA3
}

char keypad_getKey(void){
    return mapKey(readKeyPadRAW());
}

static unsigned int readKeyPadRAW(void){
    LATB |= 0b1111000000000000;

    _LATB12 = 0;
    asm("nop"); asm("nop"); asm("nop");
    if (_RA0 == 0) return 1;
    if (_RA1 == 0) return 2;
    if (_RA2 == 0) return 3;
    if (_RA3 == 0) return 4;
    _LATB12 = 1;

    _LATB13 = 0;
    asm("nop"); asm("nop"); asm("nop");
    if (_RA0 == 0) return 5;
    if (_RA1 == 0) return 6;
    if (_RA2 == 0) return 7;
    if (_RA3 == 0) return 8;
    _LATB13 = 1;

    _LATB14 = 0;
    asm("nop"); asm("nop"); asm("nop");
    if (_RA0 == 0) return 9;
    if (_RA1 == 0) return 10;
    if (_RA2 == 0) return 11;
    if (_RA3 == 0) return 12;
    _LATB14 = 1;

    _LATB15 = 0;
    asm("nop"); asm("nop"); asm("nop");
    if (_RA0 == 0) return 13;
    if (_RA1 == 0) return 14;
    if (_RA2 == 0) return 15;
    if (_RA3 == 0) return 16;
    _LATB15 = 1;

    LATB |= 0b1111000000000000;
    return 0;
}

static char mapKey(unsigned int rawKey) {
    switch (rawKey) {
        case 1:  return 'A';
        case 2:  return '3';
        case 3:  return '2';
        case 4:  return '1';
        case 5:  return 'b';
        case 6:  return '6';
        case 7:  return '5';
        case 8:  return '4';
        case 9:  return 'C';
        case 10: return '9';
        case 11: return '8';
        case 12: return '7';
        case 13: return 'd';
        case 14: return 'E';
        case 15: return '0';
        case 16: return 'F';
        default: return ' ';
    }
}
