//------------------------------------------------------------------------------
// File:          i2c.c
// Written By:    E.G. Walters
// Date Created:  14 Jan 15
// Description:   C file for I2C communications
// Compiler:	  XC32 v1.33
// Target:        PIC32MX320F128H (Digilent chipKIT Uno32 board)
// Schematic(s):  Digilent schematics for Uno32 board
//
// Revision History (date, initials, description)
//   20 Mar 18, egw100, Updated for SP18
//------------------------------------------------------------------------------
#include "i2c.h"


//------------------------------------------------------------------------------
// Function: I2C_Init()
// This function should be called in the main initialization routine to
// initialize I2C 1.
//------------------------------------------------------------------------------
void I2C_Init(void) {

    int32_t actualClock;
    I2CConfigure(I2C1, I2C_ENABLE_SLAVE_CLOCK_STRETCHING);
    actualClock = I2CSetFrequency(I2C1, 10000000, 400000); // 400 kHz
    printf("I2C actual clock = %d\n", actualClock);
    I2CEnable(I2C1, TRUE);

} // end I2C_Init()



//------------------------------------------------------------------------------
// Function: I2C_Start()
// Sends the start signal for an I2C message
//------------------------------------------------------------------------------
I2C_RESULT I2C1_Start(void) {

    I2C_RESULT result;
    I2C_STATUS status;

    while ( !I2CBusIsIdle(I2C1) ) {};
    result = I2CStart(I2C1);
    do {
        status = I2CGetStatus(I2C1);
    } while ( !(status & I2C_START) );

    return result;

} // end I2C_Start()



//------------------------------------------------------------------------------
// Function: I2C_Stop()
// Sends the stop signal for an I2C message
//------------------------------------------------------------------------------
void I2C1_Stop(void) {

    I2C_STATUS status;

    I2CStop(I2C1);
    do {
        status = I2CGetStatus(I2C1);
    } while ( !(status & I2C_STOP) );

} // end I2C_Stop()



//------------------------------------------------------------------------------
// Function: I2C_Restart()
// Sends the repeat start signal for an I2C message
//------------------------------------------------------------------------------
I2C_RESULT I2C1_Restart(void) {

    I2C_RESULT result;
    I2C_STATUS status;

    result = I2CRepeatStart(I2C1);
    do {
        status = I2CGetStatus(I2C1);
    } while ( !(status & I2C_START) );

    return result;

} // end I2C1_Restart()



//------------------------------------------------------------------------------
// Function: I2C1_SendByte()
// Sends one byte of an I2C message
//------------------------------------------------------------------------------
I2C_RESULT I2C1_SendByte(uint8_t data) {

    I2C_RESULT result;

    while ( !I2CTransmitterIsReady(I2C1) ) {};
    result = I2CSendByte( I2C1, data );
    while ( !I2CTransmissionHasCompleted(I2C1) ) {};

} // end I2C1_SendByte()



//------------------------------------------------------------------------------
// Function: I2C1_ReceiveByte()
// Receives one byte of an I2C message
//------------------------------------------------------------------------------
uint8_t I2C1_ReceiveByte(void) {

    I2C_RESULT result;

    I2CReceiverEnable(I2C1, TRUE);
    while ( !I2CReceivedDataIsAvailable(I2C1) ) {};
    I2CAcknowledgeByte(I2C1, FALSE);
    return I2CGetByte(I2C1);

} // end I2C1_ReceiveByte()



//------------------------------------------------------------------------------
// Function: I2C1_WriteRegister()
// Writes a value to a register for typical I2C device
//------------------------------------------------------------------------------
void I2C1_WriteRegister(uint8_t slaveAddress, uint8_t registerAddress,
        uint8_t data) {

    I2C_RESULT result;

    printf("starting\n");
    result = I2C1_Start();
    printf("result = %d\n", result);
    result = I2C1_SendByte(slaveAddress << 1);
    printf("result = %d\n", result);
    result = I2C1_SendByte(registerAddress);
    printf("result = %d\n", result);
    result = I2C1_SendByte(data);
    printf("result = %d\n", result);
    I2C1_Stop();

} // end I2C1_WriteRegister()



//------------------------------------------------------------------------------
// Function: I2C1_ReadRegister()
// Reads a value from a register for typical I2C device
//------------------------------------------------------------------------------
uint8_t I2C1_ReadRegister(uint8_t slaveAddress, uint8_t registerAddress) {

    uint8_t data;

    I2C1_Start();
    I2C1_SendByte(slaveAddress << 1);
    I2C1_SendByte(registerAddress);
    I2C1_Restart();
    I2C1_SendByte((slaveAddress << 1) + 1);
    data = I2C1_ReceiveByte();
    I2C1_Stop();
    return data;

} // end I2C1_ReadRegister()


