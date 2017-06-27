#include <msp430.h>
#include <math.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>

/**
Author: Brittain Cooke
Last Modified: 06272017

Summary:
 Code for an MSP430 microcontroller. Runs 2 servo motors. Motors move randomly.
 Bot goes roughly in a circle.

 Problems:
  Wheels moves at different places do to:
    Mechanical:
    - Slippage
    - Uneven powering issue(Was powering through board)
    - Caster was dragging
    Software:
    - Duty cycle may need to be adjusted

*/

/**
Globals
*/
int n =0;//Counter
//Buffers to hold different Duty cycles(Controls Motor Direction)
int *signalBuffP[4]={2000,0,1000,0};//Passanger Side Modes 2000-Forward, 1000-Reverse, 0-Stop
int *signalBuffD[4]={1000,0,1000,0};//Driver Side Modes 1000-Forward, 2000-Reverse, 0-Stop


int main(void)
{
  /* CLOCKS *********************************/
  WDTCTL   = WDTPW | WDTHOLD;               // Disable Watchdog
  BCSCTL1  = CALBC1_1MHZ;                   // Run @ 1MHz
  DCOCTL   = CALDCO_1MHZ;

  /* PINS *********************************/
  P1DIR = 7 | BIT6;//Activates for use P1.X Pins use as needed
  P1OUT = 3;// Powers P1.X Pins

  P2DIR = BIT1 | BIT4;//Activates for use P2.X Pins use as needed
  P2SEL = BIT1 | BIT4;//Selects alternate Output mode(PWM Signal) for P2.X Pins

  /* TIMER *********************************/
  //TimerA0 is activated and is used to control the interupts
  //(Controlling Time interval of the Change in direction)
  TA0CTL      = TASSEL_2 | MC_1 | ID_3;
  TA0CCR0     = 65535;
  TA0CCTL0    = CCIE;


  /* TIMER *********************************/
  //TimerA1 is activated and is used to control the PWM Duty cycle
  //(Controlling Direction)
  TA1CTL      = TASSEL_2 | MC_1 | ID_0;
  TA1CCR0     = 4000;
  TA1CCTL0    = CCIE;
  TA1CCTL1    = OUTMOD_7;
  TA1CCTL2    = OUTMOD_7;
  TA1CCR1     = signalBuffD[0];
  TA1CCR2     = signalBuffP[0];


  /* BUTTON *********************************/
  P1REN   = 8;  // enable pull-up/down resistor for P1.3
  P1OUT  |= 8;  // select pull-up resistor (not pull-down)
  P1IE   |= 8;  // enable interrupt for P1.3
  P1IES  |= 8;  // select high-to-low transition
  P1IFG  &= ~8; // clear interrupt flag

  __eint();

  for(;;){}
  return 0;                                 // Enable Global Interrupts

}

//TimerA0 interupt
#pragma vector=TIMER0_A0_VECTOR
__interrupt void blink (void)
{
  n++;
  if(n==4){
    n=0;
  }
  TA1CCR2 = signalBuffP[n];
  TA1CCR1 = signalBuffD[n];
}

//Optional button interupt INstead of timer
// #pragma vector=PORT1_VECTOR
// __interrupt void Button (void)
// {
//   P1OUT   |= BIT3;
//   n++;
//   if(n==4){
//     n=0;
//   }
//   TA1CCR2 = signalBuffP[n];
//   TA1CCR1 = signalBuffD[n];
//
//   while (!(BIT3 & P1IN)) {} // is finger off of button yet?
//   __delay_cycles(32000); // wait 32m
//   P1IFG &= ~BIT3; // clear interrupt flag
//
// }
