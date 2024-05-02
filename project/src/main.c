#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT6               // P1.0
#define LED_GREEN BIT0             // P1.6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT0		/* switch1 is p1.3 */
#define SWITCHES SW1		/* only 1 switch on this board */

void main(void) 
{  
  configureClocks();

  P2DIR |= LEDS;
  P2OUT &= ~LEDS;		/* leds initially off */
  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */

  or_sr(0x18);  // CPU off, GIE on
} 

void
switch_interrupt_handler()
{
  char p2val = P2IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */

/* up=red, down=green */
  if (p2val & SW1) {
    P1OUT |= LED_RED;
    P1OUT &= ~LED_GREEN;
  } else {
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_RED;
  }
}


/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
