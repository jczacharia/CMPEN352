//------------------------------------------------------------------------------
// File:          lab08.h
// Written By:    Jordan Hartung, Jeremy Zacharia
// Date Created:  20 Jan 15
// Description:   Test touch communication to touch screen
// Compiler:	  XC32 v1.34
// Target:        PIC32MX340F512H
// Schematic(s):  Drawing L07-01
//
// Revision History (date, initials, description)
//   9 Sep 15, egw100, Updated for EE 316 FA 15
//   24 Jan 17, egw100, Updated for CMPEN 352W SP 17
//   13 Feb 18, egw100, Updated for CMPEN 352W SP 18
//   27 Feb 18, jth5342, added lab05 requirements
//   3/14/2018, jth5342, added CS and libraries for tft and spi
//   3/28/2018  jth5342,jmz5234, added I2C function
//------------------------------------------------------------------------------
#ifndef LAB08_H
#define	LAB08_H



// PIC32MX340F512H Configuration Bit Settings
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)


// Includes
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include "spi.h"
#include "tft.h"
#include "delay.h"
#include "touch.h"
#include "adc.h"


#include "math.h"

// Defines
#define TRIS_OUTPUT 0  // Used to set tristate to output
#define TRIS_INPUT 1   // Used to set tristate to input

#define IO_CS        PORTDbits.RD1     // TFT Chip select pin (active low)
#define IO_CS_TRIS   TRISDbits.TRISD1  // TFT Chip select trisate

/*************** TIMER ***************/
uint16_t mscount, secondcount;
bool sample_temp;


/*
 * Function prototypes
 */
void MCU_initialize(void);
void _mon_putc(char c);
void __ISR(_TIMER_1_VECTOR, IPL7SOFT) Timer1Handler(void);


/*
 * Screen States
 */
enum screen_t {
    MAIN,
    JSTICK,
    TEMP
};

/*
 * Global variables
 */
char message[17];
int x, xp; // xp = y previous; for state
int y, yp; // yp = y previous; for state
uint16_t valueu;
uint16_t values;
double temp_sense;
uint8_t rad; //circle radius
enum screen_t screen; //screen states
enum screen_t old_screen;

#endif	/* LAB08_H */

