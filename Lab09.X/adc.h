//------------------------------------------------------------------------------
// File:          adc.h
// Written By:    Jordan Hartung, Jeremy Zacharia
// Date Created:  3/28/2018
// Description:   JoyStick and Temp
// Compiler:	  XC32 v1.34
// Target:        PIC32MX340F512H
// Schematic(s):  Drawing Lab9
//------------------------------------------------------------------------------


#ifndef _ADC_H
#define _ADC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <xc.h>

void ADC_init();

uint16_t ADC_sample(uint8_t pin);


#endif