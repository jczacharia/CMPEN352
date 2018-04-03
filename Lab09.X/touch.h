//------------------------------------------------------------------------------
// File:          touch.h
// Written By:    E.G. Walters
// Date Created:  28 Feb 17
// Description:   Header file for touch on TFT display
// Compiler:	  XC32 v1.34
// Target:        Adafruit TFT Resistive Touch Shield
// Schematic(s):  Adafruit schematic for TFT shield
//
// NOTE: touch.h and touch.c were originally ported from Adafruit_STMPE610
//   library downloaded from Adafruit.com
//
// Revision History (date, initials, description)
//   12 Mar 17, egw100, Added TOUCH_ClearBuffer() function
//   13 Mar 18, egw100, Updated for SP18
//------------------------------------------------------------------------------

#ifndef TOUCH_H
#define	TOUCH_H


// Includes
#include <xc.h>
#include <stdint.h>
#include "spi.h"
#include "tft.h"


// Defines
#define TOUCH_CS        PORTDbits.RD10     // Touch Chip select pin (active low)
#define TOUCH_CS_TRIS   TRISDbits.TRISD10  // Touch Chip select trisate

#define TOUCH_REG_CHIP_ID       0x00
#define TOUCH_REG_ID_VER        0x02
#define TOUCH_REG_SYS_CTRL1     0x03
#define TOUCH_REG_SYS_CTRL2     0x04
#define TOUCH_REG_TSC_CTRL      0x40
#define TOUCH_REG_INT_CTRL      0x09
#define TOUCH_REG_INT_EN        0x0A
#define TOUCH_REG_INT_STA       0x0B
#define TOUCH_REG_ADC_CTRL1     0x20
#define TOUCH_REG_ADC_CTRL2     0x21
#define TOUCH_REG_TSC_CFG       0x41
#define TOUCH_REG_FIFO_TH       0x4A
#define TOUCH_REG_FIFO_SIZE     0x4C
#define TOUCH_REG_FIFO_STA      0x4B
#define TOUCH_REG_TSC_I_DRIVE   0x58
#define TOUCH_REG_TSC_DATA_X    0x4D
#define TOUCH_REG_TSC_DATA_Y    0x4F
#define TOUCH_REG_TSC_FRACT_Z   0x56
#define TOUCH_REG_GPIO_SET_PIN  0x10
#define TOUCH_REG_GPIO_CLR_PIN  0x11
#define TOUCH_REG_GPIO_DIR      0x13
#define TOUCH_REG_GPIO_AF       0x17

#define TOUCH_MINX 150
#define TOUCH_MINY 130
#define TOUCH_MAXX 3800
#define TOUCH_MAXY 4000


// Function prototypes
extern void msDelay(uint16_t ms);
void     TOUCH_Init(void);
uint8_t  TOUCH_IsTouched(void);
uint8_t  TOUCH_BufferIsEmpty(void);
uint8_t  TOUCH_BufferSize(void);
void     TOUCH_ClearBuffer(void);
uint16_t TOUCH_GetCHIP_ID(void);
uint8_t  TOUCH_GetID_VER(void);
uint16_t TOUCH_ReadReg16(uint8_t reg);
uint8_t  TOUCH_ReadReg8(uint8_t reg);
void     TOUCH_WriteReg8(uint8_t reg, uint8_t value);

extern uint16_t touch_x;
extern uint16_t touch_y;
extern uint16_t touch_z;

#endif	/* TOUCH_H */

