#ifndef _ADC_H
#define _ADC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <xc.h>

void ADC_init();

uint16_t ADC_sample(uint8_t pin);


#endif