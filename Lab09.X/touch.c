//------------------------------------------------------------------------------
// File:          touch.c
// Written By:    E.G. Walters
// Date Created:  28 Feb 17
// Description:   C file for touch on TFT display
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
#include "touch.h"


uint8_t i;
uint8_t originalSPI2BRG;

uint16_t value16;
uint8_t  value8;

uint16_t touch_x;
uint16_t touch_y;
uint16_t touch_z;


//------------------------------------------------------------------------------
// Function: TOUCH_Init()
// Initializes the touch screen (STMPE610)
//------------------------------------------------------------------------------
void TOUCH_Init(void) {

    // Initialize CS pin for output, set it high
    TOUCH_CS_TRIS = 0;
    TOUCH_CS = 1;

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz

    TOUCH_GetCHIP_ID();

    TOUCH_WriteReg8(TOUCH_REG_SYS_CTRL1, 0x02);
    msDelay(1000);

    for (i = 0; i < 65; i++) {
        TOUCH_ReadReg8(i);
    }

    TOUCH_WriteReg8(TOUCH_REG_SYS_CTRL2, 0x00); // turn on clocks!
    TOUCH_WriteReg8(TOUCH_REG_TSC_CTRL, 0x01);  // XYZ and enable!
    TOUCH_WriteReg8(TOUCH_REG_INT_EN, 0x01);
    TOUCH_WriteReg8(TOUCH_REG_ADC_CTRL1, 0x60); // 124 clocks per conversion
    TOUCH_WriteReg8(TOUCH_REG_ADC_CTRL2, 0x02);

//    TOUCH_WriteReg8(TOUCH_REG_TSC_CFG, 0xA0); // Use for recording lots of points
    TOUCH_WriteReg8(TOUCH_REG_TSC_CFG, 0xFB); // Use for recording fewer points

    TOUCH_WriteReg8(TOUCH_REG_TSC_FRACT_Z, 0x06);
    TOUCH_WriteReg8(TOUCH_REG_FIFO_TH, 0x01);
    TOUCH_WriteReg8(TOUCH_REG_FIFO_STA, 0x01); // Reset FIFO
    TOUCH_WriteReg8(TOUCH_REG_FIFO_STA, 0x00); // Release FIFO reset
    TOUCH_WriteReg8(TOUCH_REG_TSC_I_DRIVE, 0x01);
    TOUCH_WriteReg8(TOUCH_REG_INT_STA, 0xFF); // reset all ints
    TOUCH_WriteReg8(TOUCH_REG_INT_CTRL, 0x05);

    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

} // end TOUCH_Init()


//------------------------------------------------------------------------------
// Function: TOUCH_GetTouchPoints()
// Reads touch points, saves them in touch_x, touch_y and touch_z
//------------------------------------------------------------------------------
void TOUCH_GetTouchPoints(void) {
    uint8_t data[4];
    uint16_t x, y, z;

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz

    for (i = 0; i < 4; i++) {
        data[i] = TOUCH_ReadReg8(0x57);
    }

    x = data[0];
    x <<= 4;
    x |= (data[1] >> 4);
    y = data[1] & 0x0F;
    y <<= 8;
    y |= data[2];
    z = data[3];

    touch_x = (TFT_WIDTH  * (x - TOUCH_MINX)) / (TOUCH_MAXX - TOUCH_MINX);
    touch_y = (TFT_HEIGHT * (y - TOUCH_MINY)) / (TOUCH_MAXY - TOUCH_MINY);
    touch_z = z;

    if (TOUCH_BufferIsEmpty()) {
        TOUCH_WriteReg8(TOUCH_REG_INT_STA, 0xFF); // reset all ints
    }

    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

} // end TOUCH_GetTouchPoints()


//------------------------------------------------------------------------------
// Function: TOUCH_IsTouched()
// Returns 0x80 if touched, 0x00 if not touched
//------------------------------------------------------------------------------
uint8_t TOUCH_IsTouched(void) {

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz
    value8 = (TOUCH_ReadReg8(TOUCH_REG_TSC_CTRL) & 0x80);
    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

    return value8;

} // end TOUCH_IsTouched()


//------------------------------------------------------------------------------
// Function: TOUCH_BufferIsEmpty()
// Returns 0x20 if buffer is empty, 0x00 if not empty
//------------------------------------------------------------------------------
uint8_t TOUCH_BufferIsEmpty(void) {

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz
    value8 = (TOUCH_ReadReg8(TOUCH_REG_FIFO_STA) & 0x20);
    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

    return value8;

} // end TOUCH_BufferIsEmpty()


//------------------------------------------------------------------------------
// Function: TOUCH_BufferSize()
// Returns buffer size
//------------------------------------------------------------------------------
uint8_t TOUCH_BufferSize(void) {

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz
    value8 = TOUCH_ReadReg8(TOUCH_REG_FIFO_SIZE);
    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

    return value8;

} // end TOUCH_BufferSize()


//------------------------------------------------------------------------------
// Function: TOUCH_ClearBuffer()
// Clears touch buffer
//------------------------------------------------------------------------------
void TOUCH_ClearBuffer(void) {

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    TOUCH_WriteReg8(TOUCH_REG_FIFO_STA, 0x01); // Reset FIFO
    TOUCH_WriteReg8(TOUCH_REG_FIFO_STA, 0x00); // Release FIFO reset
    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

} // end TOUCH_ClearBuffer()


//------------------------------------------------------------------------------
// Function: TOUCH_GetCHIP_ID()
// Reads 16-bit register 0x00 (CHIP_ID) and returns the value
//------------------------------------------------------------------------------
uint16_t TOUCH_GetCHIP_ID(void) {

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz
    value16 = TOUCH_ReadReg16(TOUCH_REG_CHIP_ID);
    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

    return value16;

} // end TOUCH_GetCHIP_ID()


//------------------------------------------------------------------------------
// Function: TOUCH_GetID_VER()
// Reads 16-bit register 0x00 (CHIP_ID) and returns the value
//------------------------------------------------------------------------------
uint8_t TOUCH_GetID_VER(void) {

    originalSPI2BRG = SPI2BRG; // Save SPI speed
    SPI2BRG = 4;  // Slow SPI to 1 MHz
    value8 = TOUCH_ReadReg8(TOUCH_REG_ID_VER);
    SPI2BRG = originalSPI2BRG; // Restore SPI to original speed

    return value8;

} // end TOUCH_GetID_VER()


//------------------------------------------------------------------------------
// Function: TOUCH_ReadReg16()
// Reads a 16-bit register and returns the value
// NOTE: Be sure SPI is 1 MHz or slower before calling this function
//------------------------------------------------------------------------------
uint16_t TOUCH_ReadReg16(uint8_t reg) {

    TOUCH_CS = 0;
    SPI2_TransmitByte(0x80 | reg);
    value16 = SPI2_TransmitByte(0x00);
    value16 <<= 8;
    SPI2_TransmitByte(0x80 | (reg + 1));
    value16 |= SPI2_TransmitByte(0x00);
    TOUCH_CS = 1;

    return value16;

} // end TOUCH_ReadReg16()


//------------------------------------------------------------------------------
// Function: TOUCH_ReadReg8()
// Reads an 8-bit register and returns the value
// NOTE: Be sure SPI is 1 MHz or slower before calling this function
//------------------------------------------------------------------------------
uint8_t TOUCH_ReadReg8(uint8_t reg) {

    TOUCH_CS = 0;
    SPI2_TransmitByte(0x80 | reg);
    value8 = SPI2_TransmitByte(0x00);
    TOUCH_CS = 1;

    return value8;

} // end TOUCH_ReadReg8()


//------------------------------------------------------------------------------
// Function: TOUCH_WriteReg8()
// Writes to an 8-bit register
// NOTE: Be sure SPI is 1 MHz or slower before calling this function
//------------------------------------------------------------------------------
void TOUCH_WriteReg8(uint8_t reg, uint8_t value) {

    TOUCH_CS = 0;
    SPI2_TransmitByte(reg);
    SPI2_TransmitByte(value);
    TOUCH_CS = 1;

} // end TOUCH_WriteReg8()

