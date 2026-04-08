/*
 * File:   rames180_lab5_main.c
 * Author: tarak
 * * Target: PIC24FJ64GA002
 */

#include "xc.h"
#include "rames180_lab5_asmLib.h"

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
   
#define LCD_ADDR 0x78   // 0x3C shifted left [cite: 206]

// --- Helper Functions ---

void delay(int delay_in_ms) {
    for (int num_ms = delay_in_ms; num_ms > 0; num_ms--)
        wait_tr_1ms();
}

void lcd_cmd(char cmd) {    
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN);
    _MI2C1IF = 0;
    
    I2C1TRN = 0b01111000;
    while(_MI2C1IF == 0 || _TRSTAT == 1);
    _MI2C1IF = 0;
    
    I2C1TRN = 0x00;
    while(_MI2C1IF == 0 || _TRSTAT == 1);
    _MI2C1IF = 0;
    
    I2C1TRN = cmd;
    while(_MI2C1IF == 0 || _TRSTAT == 1);
    _MI2C1IF = 0;
    
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN);
}

void lcd_printChar(char c) {
    _LATB5 = 1;
    
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN);
    _MI2C1IF = 0;
    
    I2C1TRN = 0b01111000;
    while(_MI2C1IF == 0 || _TRSTAT == 1);
    _MI2C1IF = 0;
    
    I2C1TRN = 0x40;
    while(_MI2C1IF == 0 || _TRSTAT == 1);
    _MI2C1IF = 0;
    
    I2C1TRN = c;
    while(_MI2C1IF == 0 || _TRSTAT == 1);
    _MI2C1IF = 0;
    
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1);
    
    _LATB5 = 0;
}

void lcd_setCursor(char x, char y) {
    char addr = (y * 0x20) + x; 
    lcd_cmd(0x80 | addr);
}

void lcd_init(void) {
    lcd_cmd(0x3A);
    _TRISB6 = 0;
    _LATB5 = 1;
    _LATB6 = 0;
    delay(1);
    _LATB6 = 1;
    delay(40);
    
    // Required Initialization sequence from SSD1803A/DOGS104 [cite: 34, 35, 157]
    lcd_cmd(0x3A); // Function Set (8-bit, 4 line, RE=1, IS=0)
    lcd_cmd(0x09); // 4-line display
    lcd_cmd(0x06); // Bottom view
    lcd_cmd(0x1E); // Bias setting
    lcd_cmd(0x39); // Function Set (RE=0, IS=1)
    lcd_cmd(0x1B); // Internal OSC frequency
    lcd_cmd(0x6E); // Follower control
    lcd_cmd(0x56); // Power control
    lcd_cmd(0x7A); // Contrast set (adjustable) [cite: 158]
    lcd_cmd(0x38); // Function Set (RE=0, IS=0)
    lcd_cmd(0x0F); // Display ON, Cursor ON, Blink ON
    
    _LATB5 = 0;
    
    lcd_cmd(0x3a);
    lcd_cmd(0x09);
    lcd_cmd(0x1a);
    lcd_cmd(0x3c);
    
    lcd_cmd(0x01);
    delay(2);
}

void lcd_printString(const char *str){
    while (*str != '\0'){
        lcd_printChar(*str);
        str++;
    }
}

void setup(void) {
    CLKDIVbits.RCDIV = 0;    // Set frequency to 16 MHz (FCY=16MHz) [cite: 80]
    AD1PCFG = 0xffff;        // Set all pins digital

    _TRISB5 = 0;
    _LATB5 = 0;
    
    I2C1CONbits.I2CEN = 0;
    I2C1BRG = 157; //(FCY/FSCL - FCY/10,000,000) - 1 = (16MHz/100kHz - 16MHz/10MHz) - 1
    I2C1CONbits.I2CEN = 1;
    _MI2C1IF = 0;
}

void loop(void) {
    static offset = 0;
    const char *msg = "Welcome Gophers!   "; //Length = 16
    
    lcd_cmd(0x01);
    delay(2);
    
    lcd_setCursor(0,1);
    lcd_printString("To EE2361");
    
    for(int i = 0; i < 19; i++){
        int charIndex = (offset + i) % 19;
        
        lcd_setCursor(i,0);
        lcd_printChar(msg[charIndex]);
    }
    
    offset++;
   
    if (offset >= 19){
        offset = 0;
    }
    
    delay(300);
}

int main(void) {
    setup();
    lcd_init();
    
    while (1) {
        loop();
    }
}