
#include "adc.h"

void ADC_init()
{
	/*
	 * ADC Configuration
	 */
	AD1PCFGbits.PCFG2 = 0; //AN8
	AD1PCFGbits.PCFG4 = 0; //AN4
	AD1PCFGbits.PCFG8 = 0; //AN2
	TRISBbits.TRISB2 = 1; //AN8
	TRISBbits.TRISB4 = 1; //AN4
	TRISBbits.TRISB8 = 1; //AN1
	//reset all ADC configuration bits
	AD1CON1 = 0;
	AD1CON2 = 0;
	AD1CON3 = 0;
	//begin ADC configuration
	AD1CON1bits.FORM = 0; //16-bit integer
	AD1CON1bits.SSRC = 7; //sample clock is auto convert
	AD1CON2bits.VCFG = 0; //Vref+ = AVdd; Vref- = AVss
	AD1CON2bits.CSCNA = 0; //do not scan inputs
	AD1CON2bits.BUFM = 0; //one 16-word buffer fill
	AD1CON2bits.ALTS = 0; //always use MUXA
	AD1CON3bits.ADRC = 0; //ADC clock = PBCLK
	AD1CON3bits.SAMC = 31; //sample time = max, 31 * Tad
	AD1CON3bits.ADCS = 63; //Tad = 128 * Tpb = 12.8us @ PBCLK = 10 MHZ
	AD1CON1bits.ADON = 1; //ADC on
}

uint16_t ADC_sample(uint8_t pin)
{
	AD1CHSbits.CH0SA = pin; //A1 up and down
	AD1CON1bits.SAMP = 1;
	while (!AD1CON1bits.DONE);
	return ADC1BUF0;
}