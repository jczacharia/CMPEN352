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

static long map (long x, long in_min, long in_max, long out_min, long out_max)
{
     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void screen_task()
{
     if (screen == JSTICK)
     {
          //        TFT_FillRectangle(0, 80, 240, 240, TFT_BLUE);
          //        TFT_DrawString(10, 100, "CMPEN 352W", TFT_BLACK, TFT_BLUE, 2);
          //        TFT_DrawString(10, 135, "Lab 7", TFT_YELLOW, TFT_BLUE, 2);
          //        TFT_DrawString(10, 160, "Jordan Hartung", TFT_GREEN, TFT_BLUE, 2);
          //        TFT_DrawString(10, 200, "Jeremey Zacharia", TFT_PURPLE, TFT_BLUE, 2);
          //joystick display code
          AD1CHSbits.CH0SA = 4; //A1 up and down
          AD1CON1bits.SAMP = 1;
          while (!AD1CON1bits.DONE);
          valueu = ADC1BUF0;

          AD1CHSbits.CH0SA = 2; //A0 left and right
          AD1CON1bits.SAMP = 1;
          while (!AD1CON1bits.DONE);
          values = ADC1BUF0;


          y = map((1023 - valueu), 0, 1023, 89, 311); //scale
          x = map(values, 0, 1023, 9, 231); //scale

          TFT_FillCircle(xp, yp, rad, TFT_BLUE);
          TFT_DrawCircle(x, y, rad, TFT_YELLOW);
          
          xp = x;
          yp = y;

          sprintf (message, "X: %d.0, Y: %d.0", x, y);
          TFT_DrawString (10, 100, message, TFT_BLACK, TFT_BLUE, 2);

          printf ("Val: %d\t%d\t%d\t%d\n", valueu, values, x, y);
     } else if (screen == TEMP && sample_temp == true)
     {
          AD1CHSbits.CH0SA = 8; //Temp
          AD1CON1bits.SAMP = 1;
          while (!AD1CON1bits.DONE);
          temp_sense = (double) ADC1BUF0;
          sprintf (message, "Temp in F: %.02f", (temp_sense / 10.0)*(9.0 / 5.0) + 32.0);
          TFT_DrawString (10, 100, message, TFT_BLACK, TFT_BLUE, 2);
          sample_temp = false;
     }
}

//------------------------------------------------------------------------------
// Function: main()
//------------------------------------------------------------------------------

void
main (void)
{

     MCU_initialize ();
     SPI_Init ();
     TFT_Init ();
     TOUCH_Init ();

     TFT_FillRectangle (0, 0, 240, 320, TFT_BLUE); //background color
     TFT_FillRectangle (0, 0, 75, 80, TFT_LIGHTGREY);
     TFT_FillRectangle (80, 0, 75, 80, TFT_LIGHTGREY);
     TFT_FillRectangle (160, 0, 80, 80, TFT_LIGHTGREY);
     TFT_DrawString (10, 30, "MAIN", TFT_BLACK, TFT_LIGHTGREY, 2);
     TFT_DrawString (98, 20, "JOY", TFT_BLACK, TFT_LIGHTGREY, 2);
     TFT_DrawString (88, 38, "STICK", TFT_BLACK, TFT_LIGHTGREY, 2);
     TFT_DrawString (176, 30, "TEMP", TFT_BLACK, TFT_LIGHTGREY, 2);



     while (1)
     {

          //    readData = I2C1_ReadRegister(0x20, 0x09);
          //    ledOnBtn = (readData >> 7) & (0b00000001);
          //    ledOffBtn = (readData >> 6) & (0b00000001);
          //
          //    if (((ledOffBtn == 1 && ledOnBtn == 1) || ledOffBtn == 1) && ledState == 1)
          //    {
          //      I2C1_WriteRegister(0x20, 0x0A, 0b00000000);
          //      ledState = 0;
          //    }
          //    else if (ledOnBtn == 1 && ledOffBtn == 0 && ledState == 0)
          //    {
          //      I2C1_WriteRegister(0x20, 0x0A, 0b00100000);
          //      ledState = 1;
          //    }


          TOUCH_GetTouchPoints ();
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
          case MAIN: //main sceen
               if (oldScreen != screen)
               {
                    TFT_FillRectangle (0, 80, 240, 240, TFT_BLUE);
                    TFT_DrawString (10, 100, "CMPEN 352W", TFT_BLACK, TFT_BLUE, 2);
                    TFT_DrawString (10, 135, "Lab 7", TFT_YELLOW, TFT_BLUE, 2);
                    TFT_DrawString (10, 160, "Jordan Hartung", TFT_GREEN, TFT_BLUE, 2);
                    TFT_DrawString (10, 200, "Jeremy Zacharia", TFT_PURPLE, TFT_BLUE, 2);
               }
               break;
          case JSTICK: //led screen
               //Taken care of in screen_task()
               if (oldScreen != screen)
               {
                    TFT_FillRectangle (0, 80, 240, 240, TFT_BLUE); //Clear
               }
               break;

          case TEMP:
               //Taken care of in screen_task()
               if (oldScreen != screen)
               {

                    TFT_FillRectangle (0, 80, 240, 240, TFT_BLUE); //Clear
               }
               break;
          }
          oldScreen = screen;

          /******* TASKS *******/
          screen_task ();

     }

} // end main()


//------------------------------------------------------------------------------
// Function: MCU_initialize()
// Params:   void
// Returns:  void
// Initialization code for the application
//------------------------------------------------------------------------------

void MCU_initialize (void)
{

     // Use multi-vector interrupt mode

     INTConfigureSystem (INT_SYSTEM_CONFIG_MULT_VECTOR);

     // Configure oscillators
     OSCCONbits.PBDIV = 0b11; // PBCLK is SYSCLK/8 = 10 MHz

     // Init UART 1 to communicate with PC
     U1MODEbits.UEN = 0b00;
     U1MODEbits.PDSEL = 0b00; // 8-bit data, no parity
     U1MODEbits.STSEL = 0; // 1 stop bit
     U1BRG = 10; // PBCLK = 10 MHz so Baud rate = 56k
     U1STAbits.UTXEN = 1;
     U1STAbits.URXEN = 1;
     U1MODEbits.ON = 1; // Turn on UART
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
     sample_temp = true;
     screen = JSTICK;

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
     INTEnableInterrupts ();


} // end MCU_initialize()


//------------------------------------------------------------------------------
// Function: _mon_putc()
// Params:   c; character to send to stdout
// Returns:  void
// Redfine _mon_putc so stdout goes to UART
//------------------------------------------------------------------------------

void
_mon_putc (char c)
{

     while (U1STAbits.UTXBF); // wait for transmit buffer to be available
     U1TXREG = c;
} // end _mon_putc()



//------------------------------------------------------------------------------
// Function: Timer1Handler()
// Params: void
// Returns: void
// Interrupt handler for Timer 1
//------------------------------------------------------------------------------

void __ISR (_TIMER_1_VECTOR, IPL7SOFT)
Timer1Handler (void)
{
     IFS0bits.T1IF = 0; // Clear interrupt flag
     mscount++;
     if (mscount == 1000)
     {
          secondcount++;
          if (secondcount % 4 == 0) // every 4 seconds
          {
               sample_temp = true;
          }
          mscount = 0;
     }

}

