/*
 * File:   circular_buffer.c
 * Author: tarak
 *
 * Created on April 6, 2026, 12:14 AM
 */


#include "xc.h"

#define BUFFER_SIZE 5

volatile int buffer[BUFFER_SIZE];
volatile int index = 0;

void initBuffer(){
    for (int i=0; i<BUFFER_SIZE; i++){
        buffer[i] = 0;
    }
}

void putVal(int newValue) {
    buffer[index] = newValue;
    index = (index + 1) % BUFFER_SIZE;
}

int getAvg() {
    int sum = 0;
    for (int i=0; i<BUFFER_SIZE; i++){
        sum += buffer[i];
    }
    return sum / BUFFER_SIZE;
}