/*
 * File:   rames180_lab4_main.c
 * Author: tarak
 *
 * Created on March 2, 2026, 1:57 AM
 */


#include "xc.h"
#include "rames180_lab4_asmLib_v001.h"

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

//#define LAB_4A

void delay(int delay_in_ms) {
    for (int num_ms = delay_in_ms; num_ms > 0; num_ms--)
        wait_tr_1ms();
}

void initServo(void)
{
    TRISBbits.TRISB6 = 0;
    
   __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
  RPOR3bits.RP6R = 18;  // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
  __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    OC1CON = 0;
    OC1CONbits.OCTSEL = 0b1;
    OC1CONbits.OCM = 0b110;
    
    T3CON = 0;
    T3CONbits.TCKPS = 0b01;
    TMR3 = 0;
    PR3 = 39999; //20ms
    
    T3CONbits.TON = 1;
}

void setServo(int Val)
{
    OC1RS = Val;
}

void setup(void){
    _RCDIV = 0; //set frequency to 16 MHz
    AD1PCFG = 0xffff; //set all pins digital
}

#ifdef LAB_4A
    void initPushButton(void) {
        TRISBbits.TRISB8 = 1;
        CNPU2bits.CN22PUE = 1;

        T2CON = 0;
        T2CONbits.TCKPS = 0b11;
        PR2 = 62499; //1 sec
        TMR2 = 0;

        _T2IE = 1;
        _T2IF = 0;
        T2CONbits.TON = 1;
    }

    volatile int overflow = 0; //initialize overflow

    void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
        _T2IF = 0;
        overflow++;
    }

    int main(void) {
        setup();
        initServo();
        initPushButton();

        int prevState = 1;

        long int time_press1 = 0;
        long int time_press2 = 0;

        setServo(40000 * 0.09);

        while(1)
        {
            int curState = _RB8;
            if (curState == 0 && prevState == 1) {
                time_press1 = time_press2;
                time_press2 = (TMR2+((long int)(62499+1)*overflow));

                if ((time_press2 - time_press1) <= 31250) { // double click within 500 ms
                    setServo(39999 * 0.06);
                }
            }

            if (((TMR2+((long int)(62499+1)*overflow)) - time_press2) >= 125000) { // if there is no click within 2 seconds
                setServo(39999 * 0.09);
            }

            prevState = curState;
            delay(2);
        }
    }
#else
    void initPushButton(void) {
        TRISBbits.TRISB8 = 1;
        CNPU2bits.CN22PUE = 1;

        // Timer2 setup (same time base as working version)
        T2CON = 0;
        T2CONbits.TCKPS = 0b11;   // 1:256 prescaler
        PR2 = 62499;              // 1 second period
        TMR2 = 0;

        _T2IE = 1;
        _T2IF = 0;
        T2CONbits.TON = 1;

        // Input Capture PPS mapping (RB8 -> IC1)
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPINR7bits.IC1R = 8;                    // RP8 = RB8
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS

        // Input Capture setup
        IC1CON = 0;
        IC1CONbits.ICTMR = 1;     // Use Timer2
        IC1CONbits.ICM = 0b010;   // Capture on falling edge only

        _IC1IF = 0;
        _IC1IE = 1;
    }

    volatile long int overflow = 0;

    #define BUFFER_SIZE 5

    volatile long int buffer[BUFFER_SIZE];
    volatile int head = 0;
    volatile int tail = 0;
    volatile int count = 0;

    void buffer_put(long int data) {
        if (count == BUFFER_SIZE) return;

        buffer[head] = data;
        head = (head + 1) % BUFFER_SIZE;
        count++;
    }

    long int buffer_get() {
        if (count == 0) return -1;

        long int data = buffer[tail];
        tail = (tail + 1) % BUFFER_SIZE;
        count--;
        return data;
    }

    void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
        _T2IF = 0;
        overflow++;
    }

    void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void) {
        _IC1IF = 0;

        long int timestamp = IC1BUF + ((long int)(PR2 + 1) * overflow);

        buffer_put(timestamp);
    }

    int main(void) {
        setup();
        initServo();
        initPushButton();

        long int t1 = 0;
        long int t2 = 0;
        long int t3 = 0;

        setServo(40000 * 0.09);

        while (1) {
            long int eventTime = buffer_get();

            // Process new button press event
            if (eventTime > 0) {
                t1 = t2;
                t2 = t3;
                t3 = eventTime;

                // Double click within 500 ms
                if (((t2 - t1) <= 31250) && ((t3 - t2) <= 31250)) {
                    delay(200);
                    setServo(39999 * 0.06);
                }
            }

            // Current time using same time base
            long int currentTime = TMR2 + ((long int)(PR2 + 1) * overflow);

            // No click within 2 seconds
            if ((currentTime - t3) >= 125000) {
                setServo(39999 * 0.09);
            }
        }
    }
#endif
