/*
 * File:   rames180_lab2b_main_v001.c
 * Author: tarak
 *
 * Created on February 8, 2026, 9:28 PM
 */


#include "xc.h"
#include "rames180_lab2b_asmLib_v001.h"

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


#define PERIOD 10


void setup(void) {
    CLKDIVbits.RCDIV = 0;
    
    AD1PCFG = 0x9fff;
    TRISA = 0xfffe;
    LATA = 0x0000;
}

void writeColor(int r, int g, int b) {
    
    for (int i=0; i<8; i++) {
        if ((r << i) & 0x80)
            write_1();
        else
            write_0();
    }
    
    for (int j=0; j<8; j++) {
        if ((g << j) & 0x80)
            write_1();
        else
            write_0();
    }
    
    for (int k=0; k<8; k++) {
        if ((b << k) & 0x80)
            write_1();
        else
            write_0();
    }
    wait_tr_100us();
}

// LAB REPORT: Hard-coded loop takes around 453-456 CYCLES to write 24 bits
// LAB REPORT: Function takes around 790 CYCLES to write 24 bits

void delay(int delay_in_ms) {
    for (int num_ms = delay_in_ms; num_ms > 0; num_ms--) {
        wait_tr_1ms();
    }
}

void showGradient() {
    for (int byteFrameNumber = 0; byteFrameNumber <= 255; byteFrameNumber++) {
        writeColor(255 - byteFrameNumber, 0, byteFrameNumber);
        delay(PERIOD);
    }
    for (int byteFrameNumber = 255; byteFrameNumber >= 0; byteFrameNumber--) {
        writeColor(255 - byteFrameNumber, 0, byteFrameNumber);
        delay(PERIOD);
    }
}

unsigned long int packColor(uint8_t Red, uint8_t Grn, uint8_t Blu) {
    return ((long) Red << 16) | ((long) Grn << 8) | ((long) Blu);
}

uint8_t getR(unsigned long int RGBval) {
    return(uint8_t) (RGBval >> 16);
}

uint8_t getG(unsigned long int RGBval) {
    return(uint8_t) (RGBval >> 8);
}

uint8_t getB(unsigned long int RGBval) {
    return(uint8_t) (RGBval >> 0);
}

void writePacCol(unsigned long int PackedColor) {
    writeColor(getR(PackedColor), getG(PackedColor), getB(PackedColor));
}

unsigned long int Wheel(uint8_t WheelPos) {
    WheelPos = 255 - WheelPos;
    
    if(WheelPos < 85) {
        return packColor(255 - WheelPos * 3, 0, WheelPos * 3);
      }
    
    if(WheelPos < 170) {
      WheelPos -= 85;
      return packColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    
    WheelPos -= 170;
    return packColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int main(void) {
    setup();
    while (1) {
//        for (unsigned char WheelPos = 0; WheelPos <= 255; WheelPos++) {
//            unsigned long int PackedColor = Wheel(WheelPos);
//            writePacCol(PackedColor);
//            delay(PERIOD);
//        }
        showGradient();
//        writeColor(100,0,0);
    }
}
