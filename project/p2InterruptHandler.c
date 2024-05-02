#include <msp430.h>
#include <switches.h>

//switches on p2
void _interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
