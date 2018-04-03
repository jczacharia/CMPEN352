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

void main(void)
{


  MCU_initialize();
  SPI_Init();
  TFT_Init();
  TOUCH_Init();

  TFT_FillRectangle(0, 0, 240, 320, TFT_BLUE); //background color
  TFT_FillRectangle(0, 0, 75, 80, TFT_LIGHTGREY);
  TFT_FillRectangle(80, 0, 75, 80, TFT_LIGHTGREY);
  TFT_FillRectangle(160, 0, 80, 80, TFT_LIGHTGREY);
  TFT_DrawString(10, 30, "MAIN", TFT_BLACK, TFT_LIGHTGREY, 2);
  TFT_DrawString(88, 30, "23008", TFT_BLACK, TFT_LIGHTGREY, 2);
  TFT_DrawString(165, 30, "LIS3DH", TFT_BLACK, TFT_LIGHTGREY, 2);



  while (1)
  {

    readData = I2C1_ReadRegister(0x20, 0x09);
    ledOnBtn = (readData >> 7) & (0b00000001);
    ledOffBtn = (readData >> 6) & (0b00000001);

    if (((ledOffBtn == 1 && ledOnBtn == 1) || ledOffBtn == 1) && ledState == 1)
    {
      I2C1_WriteRegister(0x20, 0x0A, 0b00000000);
      ledState = 0;
    }
    else if (ledOnBtn == 1 && ledOffBtn == 0 && ledState == 0)
    {
      I2C1_WriteRegister(0x20, 0x0A, 0b00100000);
      ledState = 1;
    }


    TOUCH_GetTouchPoints();
    if (touch_x > 0 && touch_x < 75 && touch_y > 0 && touch_y < 80)
    {
      screen = 0;
    } else if (touch_x > 80 && touch_x < 155 && touch_y > 0 && touch_y < 80)
    {
      screen = 1;
    } else if (touch_x > 160 && touch_x < 240 && touch_y > 0 && touch_y < 80)
    {
      screen = 2;
    }

    switch (screen) {
    case 0: //main sceen
      if (oldScreen != screen)
      {
        TFT_FillRectangle(0, 80, 240, 240, TFT_BLUE);
        TFT_DrawString(10, 100, "CMPEN 352W", TFT_BLACK, TFT_BLUE, 2);
        TFT_DrawString(10, 135, "Lab 7", TFT_YELLOW, TFT_BLUE, 2);
        TFT_DrawString(10, 160, "Jordan Hartung", TFT_GREEN, TFT_BLUE, 2);
        TFT_DrawString(10, 200, "Jeremey Zacharia", TFT_PURPLE, TFT_BLUE, 2);
      }
      break;
    case 1: //led screen
      if (oldScreen != screen)
      {
        TFT_FillRectangle(0, 80, 240, 240, TFT_BLUE);
        TFT_DrawRectangle(79, 150, 81, 100, TFT_BLACK); //making the led picture
        TFT_DrawRectangle(70, 249, 90, 10, TFT_BLACK);
        TFT_DrawVLine(90, 258, 60, TFT_BLACK);
        TFT_DrawVLine(150, 258, 40, TFT_BLACK);
        TFT_DrawCircle(119, 150, 40, TFT_BLACK);
      }
      if (ledState == 0)
      {
        TFT_FillCircle(119, 150, 39, TFT_DARKGREEN); //clear led design
        TFT_FillRectangle(80, 150, 79, 97, TFT_DARKGREEN);
        TFT_FillRectangle(71, 250, 88, 8, TFT_DARKGREEN);
      } else if (ledState == 1)
      {
        TFT_FillCircle(119, 150, 39, TFT_GREEN); //fill the led with red
        TFT_FillRectangle(80, 150, 79, 97, TFT_GREEN);
        TFT_FillRectangle(71, 249, 88, 9, TFT_GREEN);
      }
      break;
      //                 case 2:
      //                     sprintf(message, "X: %4d", )
      //                     tft_DrawString(10,100, "")

    }

    oldScreen = screen;


  }

} // end main()


//------------------------------------------------------------------------------
// Function: MCU_initialize()
// Params:   void
// Returns:  void
// Initialization code for the application
//------------------------------------------------------------------------------

void MCU_initialize(void)
{

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

void _mon_putc (char c)
{
  while (U1STAbits.UTXBF);  // wait for transmit buffer to be available
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

