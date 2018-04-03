//------------------------------------------------------------------------------
// File:          delay.h
// Written By:    jcz
// Date Created:  20 Jan 15
// Description:   First program to work with PIC32 on Uno32 board
// Compiler:	  XC32 v1.34
// Target:        PIC32MX340F512H
// Schematic(s):  Drawing L05-01
//------------------------------------------------------------------------------
#ifndef DELAY_H
#define	DELAY_H
// Includes
#include <xc.h>
#include <stdint.h>

void usDelay(uint16_t us);
void msDelay(uint16_t ms);


#endif	/* DELAY_H */

