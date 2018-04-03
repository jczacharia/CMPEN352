//------------------------------------------------------------------------------
// File:          tft.h
// Written By:    E.G. Walters
// Date Created:  5 Jan 15
// Description:   Header file for TFT display
// Compiler:	  XC32 v1.34
// Target:        Adafruit TFT Capacitive Touch Shield
// Schematic(s):  Adafruit schematic for TFT shield
//
// NOTE: tft.h and tft.c were originally ported from Adafruit_TFT-master
//   library downloaded from Adafruit.com
//
// Revision History (date, initials, description)
//   27 Feb 18, egw100, Updated for SP18
//------------------------------------------------------------------------------
#ifndef TFT_H
#define	TFT_H


// Includes
#include <xc.h>
#include <stdint.h>


// Defines
#define TFT_BACKLIGHT PORTDbits.RD1  // TFT backlight
#define TFT_CS        PORTGbits.RG9     // TFT Chip select pin (active low)
#define TFT_CS_TRIS   TRISGbits.TRISG9  // TFT Chip select trisate
#define TFT_DC        PORTDbits.RD3     // TFT Data/Cmd pin (1 = data, 0 = command)
#define TFT_DC_TRIS   TRISDbits.TRISD3  // TFT Data/Cmd tristate

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_NOP     0x00
#define TFT_SWRESET 0x01
#define TFT_RDDID   0x04
#define TFT_RDDST   0x09

#define TFT_SLPIN   0x10
#define TFT_SLPOUT  0x11
#define TFT_PTLON   0x12
#define TFT_NORON   0x13

#define TFT_RDMODE     0x0A
#define TFT_RDMADCTL   0x0B
#define TFT_RDPIXFMT   0x0C
#define TFT_RDIMGFMT   0x0A
#define TFT_RDSELFDIAG 0x0F

#define TFT_INVOFF   0x20
#define TFT_INVON    0x21
#define TFT_GAMMASET 0x26
#define TFT_DISPOFF  0x28
#define TFT_DISPON   0x29

#define TFT_CASET   0x2A
#define TFT_PASET   0x2B
#define TFT_RAMWR   0x2C
#define TFT_RAMRD   0x2E

#define TFT_PTLAR   0x30
#define TFT_MADCTL  0x36
#define TFT_PIXFMT  0x3A

#define TFT_FRMCTR1 0xB1
#define TFT_FRMCTR2 0xB2
#define TFT_FRMCTR3 0xB3
#define TFT_INVCTR  0xB4
#define TFT_DFUNCTR 0xB6

#define TFT_PWCTR1  0xC0
#define TFT_PWCTR2  0xC1
#define TFT_PWCTR3  0xC2
#define TFT_PWCTR4  0xC3
#define TFT_PWCTR5  0xC4
#define TFT_VMCTR1  0xC5
#define TFT_VMCTR2  0xC7

#define TFT_RDID1   0xDA
#define TFT_RDID2   0xDB
#define TFT_RDID3   0xDC
#define TFT_RDID4   0xDD

#define TFT_GMCTRP1 0xE0
#define TFT_GMCTRN1 0xE1

// Color definitions (RGB 5-6-5)
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */
#define TFT_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TFT_PINK        0xF81F



// Function prototypes
extern void msDelay(uint16_t ms);
void TFT_Init(void);
void TFT_WriteCommandByte(uint8_t cmd);
void TFT_WriteDataByte(uint8_t data);
void TFT_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void TFT_DrawPixel(int16_t x, int16_t y, uint16_t color);
void TFT_FillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void TFT_DrawChar(int16_t x, int16_t y, uint8_t character, uint16_t charColor,
        uint16_t backColor, uint8_t size);
void TFT_DrawString(int16_t x, int16_t y, const uint8_t *str, uint16_t charColor,
        uint16_t backColor, uint8_t size);
void TFT_DrawVLine(int16_t x, int16_t y, int16_t height, uint16_t color);
void TFT_DrawHLine(int16_t x, int16_t y, int16_t width, uint16_t color);
void TFT_SetRotation(uint8_t rotation);
void TFT_InvertDisplay(uint8_t invert);
void TFT_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void TFT_DrawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void TFT_DrawCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
void TFT_FillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
void TFT_FillCircleHelper(int16_t x0, int16_t y0, int16_t radius,
        uint8_t cornername, int16_t delta, uint16_t color);
void TFT_DrawCircleHelper( int16_t x0, int16_t y0, int16_t radius,
        uint8_t cornername, uint16_t color);
void TFT_DrawRoundRectangle(int16_t x, int16_t y, int16_t w, int16_t h,
        int16_t radius, uint16_t color);
void TFT_FillRoundRectangle(int16_t x, int16_t y, int16_t w, int16_t h,
        int16_t radius, uint16_t color);
void TFT_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        int16_t x2, int16_t y2, uint16_t color);
void TFT_FillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        int16_t x2, int16_t y2, uint16_t color);
void TFT_DrawMonochromeSprite(int16_t x, int16_t y, int16_t width, int16_t height,
        uint8_t *sprite, uint16_t spriteColor, uint16_t backColor);



// Externs
extern const unsigned char font[];



#endif	/* TFT_H */

