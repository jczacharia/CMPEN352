//------------------------------------------------------------------------------
// File:          LIS3DH.c
// Written By:    jmz5234
// Date Created:  March 26, 2018
// Description:   I2C for LIS3DH Accelerometer
// Compiler:	  XC32 v1.34
// Target:        PIC32MX320F128H (Digilent chipKIT Uno32 board)
// Schematic(s):  Drawing L08-01
//------------------------------------------------------------------------------

#include "LIS3DH.h"
//#include "i2c.h"

void LIS3DH_init(void)
{
	//I2C_Init();

	uint8_t regRead;
	regRead = I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_WHOAMI);
	if (regRead != 0x33) {
		printf("\nERROR: ID is %x\tShould be: 0x33", regRead);
	} else {
		printf("Accelerometer OK\n\n");
	}
	//enable all axis 400Hz Low-power mode Normal
	uint8_t ctl1_reg = I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_CTRL1);
	ctl1_reg &= ~(0xF0) | (0b0111 << 4);
	I2C1_WriteRegister(LIS3DH_ADDR, LIS3DH_REG_CTRL1, ctl1_reg);

	//High resolution
	I2C1_WriteRegister(LIS3DH_ADDR, LIS3DH_REG_CTRL4, 0x88);
}

void LIS3DH_Accel_Read_XYZ(LIS3DH_accel_t *a)
{

	uint8_t statusReg;

	do // make sure LIS3DH is able to be read
	{
		statusReg = I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_STATUS1);
	} while ((statusReg & 0x03) != 0x03);

	//read upper and lower x
	a->x = (uint16_t) I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_OUT_X_L);
	a->x |= ((uint16_t) I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_OUT_X_H)) << 8;

	//read upper and lower y
	a->y = (uint16_t) I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_OUT_Y_L);
	a->y |= ((uint16_t) I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_OUT_Y_H)) << 8;

	//read upper and lower z
	a->z = (uint16_t) I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_OUT_Z_L);
	a->z |= ((uint16_t) I2C1_ReadRegister(LIS3DH_ADDR, LIS3DH_REG_OUT_Z_H)) << 8;

	//use this for dividing to get exact mag = 1
	uint16_t div = 15800; //kept changing this until accel.mag was around 1
	//15800 was okay

	a->x_g = (float) a->x / div;
	a->y_g = (float) a->y / div;
	a->z_g = (float) a->z / div;

	a->mag = sqrt(pow(a->x_g, 2) + pow(a->y_g, 2) + pow(a->z_g, 2));
}