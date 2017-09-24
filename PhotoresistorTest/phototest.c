#include <msp430.h>
#include <math.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>

#include "dtc.h"

/**
Author: Brittain Cooke
Last Modified: 06272017

Summary:


 Problems:

*/

/**
Globals
*/
unsigned int number =0;


int main(void)
{

  /* CLOCKS *********************************/
  WDTCTL   = WDTPW | WDTHOLD;               // Disable Watchdog
  BCSCTL1  = CALBC1_1MHZ;                   // Run @ 1MHz
  DCOCTL   = CALDCO_1MHZ;

  /* PINS *********************************/
  P1DIR    = BIT6;

  P2DIR    = BIT1;
  P2SEL    = BIT1;

  /* TIMER *********************************/
  TA0CTL      = TASSEL_2 | MC_1 | ID_0;
  TA0CCR0     = 1024;
  TA0CCTL0    = CCIE;

  TA1CTL      = TASSEL_2 | MC_1 | ID_0;
  TA1CCR0     = 1024;
  TA1CCTL0    = CCIE;
  TA1CCTL1    = OUTMOD_7;


  initialize_dtc(INCH_4,&number);
/* Convert from decimal to hex *********************************/

__eint();

for(;;){}
return 0;                           // Enable Global Interrupts
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
  TA1CCR1 = number;

  if(number>1000){
    P1OUT =  BIT6;
  }else{
    P1OUT & ~BIT6;
  }
}
