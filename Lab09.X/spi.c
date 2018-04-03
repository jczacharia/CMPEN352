//------------------------------------------------------------------------------
// File:          spi.c
// Written By:    E.G. Walters
// Date Created:  7 Oct 15
// Description:   C file for SPI communications
// Compiler:	  XC32 v1.34
// Target:        PIC32MX340F512H (Digilent chipKIT uC32 board)
// Schematic(s):  Digilent schematics for uC32 board
//
// Revision History (date, initials, description)
//   27 Feb 18, egw100, Updated for SP18
//------------------------------------------------------------------------------
#include "spi.h"


//------------------------------------------------------------------------------
// Function: SPI_Init()
// Params:   void
// Returns:  void
// This function should be called in the main initialization routine to
// initialize SPI 2.
//------------------------------------------------------------------------------
void SPI_Init(void) {

    SPI2CON = 0;
    SPI2BRG = 0; // 5Mhz with 10Mhz PB clock
    SPI2STATbits.SPIROV = 0;
    SPI2CONbits.CKP = 0;
    SPI2CONbits.CKE = 1;
    SPI2CONbits.SMP = 1;
    SPI2CONbits.MSTEN = 1;
    SPI2CONbits.ON = 1;

} // end SPI_Init()



//------------------------------------------------------------------------------
// Function: SPI_TransmitByte()
// Params:   bVal; the byte to transmit
// Returns:  the byte received
// Transmits one byte using the SPI #2 peripheral
//------------------------------------------------------------------------------
uint8_t SPI2_TransmitByte(uint8_t bVal) {

    // Wait for transmit buffer to be empty (ready to send new byte)
    while (SPI2STATbits.SPITBE == 0);

    // Put the byte in the transmit buffer (Transmit the byte)
    SPI2BUF = bVal;

    // Wait for receive buffer to be full (new byte has been received)
    while (SPI2STATbits.SPIRBF == 0);

    // Return the received byte
    return SPI2BUF;

} // end SPI_TransmitByte()

