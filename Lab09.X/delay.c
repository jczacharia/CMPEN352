/*
 * File:   delay.c
 * Author: Jeremy
 *
 * Created on February 17, 2018, 9:15 PM
 */


#include "delay.h"


void usDelay(uint16_t us)
{
    uint16_t doneValue = us * 10; // Timer4 counts 10 times per microsecond
    TMR4 = 0;
    while (TMR4 < doneValue); // wait for time to pass
}

void msDelay(uint16_t ms)
{
    usDelay(ms * 1000);
}