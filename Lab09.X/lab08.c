//------------------------------------------------------------------------------
// File:          lab08.c
// Written By:    Jordan Hartung, Jeremy Zacharia
// Date Created:  3/28/2018
// Description:   Test touch communication to touch screen
// Compiler:	  XC32 v1.34
// Target:        PIC32MX340F512H
// Schematic(s):  Drawing L08-01
//
// Revision History (date, initials, description)
//   3/28/2018, jth5342, initial design
//------------------------------------------------------------------------------
#include "lab08.h"

//------------------------------------------------------------------------------
// Function: main()
//------------------------------------------------------------------------------
void main(void) {


    MCU_initialize();
    SPI_Init();
    TFT_Init();
    TOUCH_Init();

    TFT_FillRectangle(0, 0, 240, 320, TFT_BLUE); //background color
    TFT_FillRectangle(0,0, 75, 80, TFT_LIGHTGREY);
    TFT_FillRectangle(80,0, 75, 80, TFT_LIGHTGREY);
    TFT_FillRectangle(160,0, 80, 80, TFT_LIGHTGREY);
    TFT_DrawString(10,30,"MAIN",TFT_BLACK,TFT_LIGHTGREY,2);
    TFT_DrawString(88,30,"23008",TFT_BLACK,TFT_LIGHTGREY,2);
    TFT_DrawString(165,30,"LIS3DH",TFT_BLACK,TFT_LIGHTGREY,2);



    while(1) {




	//joystick display code
    AD1CHSbits.CH0SA = 4; //A1 up and down
    AD1CON1bits.SAMP = 1; 
    while(!AD1CON1bits.DONE) {}
    valueu = ADC1BUF0;

    AD1CHSbits.CH0SA = 2; //A0 left and right
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE) {}
    values = ADC1BUF0;
    

    y = valueu/3.196; //scale
    x = values/4.2625; //scale

    if((x-rad) < 9) // keep circle on screen
    {
        xc = 9;
    }
    else if((x+rad) > 231)
    {
        xc = 231;
    }
    else
    {
        xc = x;
    }

    if((y-rad) < 89)
    {
        yc = 89;
    }
    else if((y+rad) > 311)
    {
        yc = 311;
    }
    else
    {
        yc = y;
    }
    if((oxc != xc) || (oyc != yc))
    {
        TFT_FillCircle(oxc, oyc, rad, TFT_BLUE);
        TFT_DrawCircle(xc, yc, rad, TFT_YELLOW);
    }
    oxc = xc;
    oyc = yc;

    sprintf(message, "X: %d, Y: %d",xc,yc);
    TFT_DrawString(10,100,message,TFT_BLACK,TFT_BLUE,2);

    printf("Val: %d\t%d\t%d\t%d\n", valueu, values,x,y);


    }

} // end main()


//------------------------------------------------------------------------------
// Function: MCU_initialize()
// Params:   void
// Returns:  void
// Initialization code for the application
//------------------------------------------------------------------------------
void MCU_initialize(void) {

    // Use multi-vector interrupt mode
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    // Configure oscillators
    OSCCONbits.PBDIV = 0b11;  // PBCLK is SYSCLK/8 = 10 MHz

    // Init UART 1 to communicate with PC
    U1MODEbits.UEN = 0b00;
    U1MODEbits.PDSEL = 0b00;  // 8-bit data, no parity
    U1MODEbits.STSEL = 0;  // 1 stop bit
    U1BRG = 10;  // PBCLK = 10 MHz so Baud rate = 56k
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;  // Turn on UART
    // Configure Timer1 to interrupt every 1 ms
    T1CON = 0x00; // Disable timer, prescale = 1:1, source = PBCLK
    TMR1 = 0; // Clear timer count
    PR1 = 10000; // PBCLK is 10 MHz, so 10000 counts = 1 kHz
    IPC1bits.T1IP = 6; // Interrupt priority = 7 (highest priority)
    IPC1bits.T1IS = 3; // Subpriority = 3 (highest)
    IFS0bits.T1IF = 0; // Clear interrupt flag
    IEC0bits.T1IE = 1; // Enable interrupt
    T1CONbits.ON = 1; // Enable timer
	


 

  //TIMER4 @ 1ms
  T4CON = 0x00; // Disable timer, prescale = 1:1, source =PBCLK
  T4CONbits.ON = 1; // enable timer
  TMR4 = 55536; // Initialize Timer4 value

    // Configure tristates
    IO_CS_TRIS = TRIS_OUTPUT; // select for IO expander

  mscount = secondcount = 0;

//ADC
  AD1PCFGbits.PCFG1 = 0;
  AD1PCFGbits.PCFG2 = 0;
  TRISBbits.TRISB1 = 1;
  TRISBbits.TRISB2 = 1;
  AD1CON1 = 0;
  AD1CON2 = 0;
  AD1CON3 = 0;

  AD1CON1bits.FORM = 0;
  AD1CON1bits.SSRC = 7;

  AD1CON2bits.VCFG = 0;
  AD1CON2bits.CSCNA = 0;
  AD1CON2bits.BUFM = 0;
  AD1CON2bits.ALTS = 0;

  AD1CON3bits.ADRC = 0;
  AD1CON3bits.SAMC = 31;
  AD1CON3bits.ADCS = 63;

  AD1CON1bits.ADON = 1;
	

    // Enable global interrupts
    INTEnableInterrupts();


} // end MCU_initialize()


//------------------------------------------------------------------------------
// Function: _mon_putc()
// Params:   c; character to send to stdout
// Returns:  void
// Redfine _mon_putc so stdout goes to UART
//------------------------------------------------------------------------------
void _mon_putc (char c) {
    while(U1STAbits.UTXBF);  // wait for transmit buffer to be available
    U1TXREG = c;
} // end _mon_putc()



//------------------------------------------------------------------------------
// Function: Timer1Handler()
// Params: void
// Returns: void
// Interrupt handler for Timer 1
//------------------------------------------------------------------------------

void __ISR (_TIMER_1_VECTOR, IPL7SOFT) Timer1Handler (void)
{
  IFS0bits.T1IF = 0; // Clear interrupt flag
  mscount++;
  if (mscount == 1000)
  {
    //printf("\n\nTIMER4 Seconds Passed: %d\n\n", secondcount++);
    mscount = 0;
  }

}

