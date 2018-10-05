#include <msp430.h>
#include <msp430g2553.h>
volatile unsigned int TC1 = 0;
int main(void)
{
WDTCTL = WDTPW | WDTHOLD;                       // Disable the watchdog timer
BCSCTL3 = LFXT1S_2;                             // Enables internal oscillator (This wasn't relavent before but yeah Dr. Google is my boi)
P1DIR |= BIT0;                                  // Set P1.0 to output (green LED)
P1REN |= BIT3;                                  // Enable internal resistor (Still don't really know how this actually works)
P1OUT |= BIT3;                                  // Enable Pull-up Resistor (Don't actually know what this is but its what Dr. Google does)
P1IE |= BIT3;                                   // Set P1.3 (button) as interrupt pin
P1IES |= BIT3;                                  // Set P1.3 to interrupt on falling edge
TACCTL0 = CCIE;
TA0CCTL0 = CCIE;                                // Capture Compare Interrupt Enabled
TACCR0 = 500;
TA0CCR0 = 500;                                  // Capture Compare Interrupt Enabled// Set Register
TACTL = TASSEL_1 + MC_1 + ID_3;
TA0CTL = TASSEL_1 + MC_0 + ID_3;                // Timer A Control: ACLK, Up Mode, Internal Divider
__enable_interrupt();                           // Enables interrupt algorithm
for(;;){};                                      // Continuous loop (lets program run and only on interrupts will it change anything)
}
#pragma vector=TIMER0_A0_VECTOR                 // This interrupt is based off TIMER A0 and blinks the light at the given interval
__interrupt void Timer_A(void)
{
    TACTL = MC_0;                               // Stop TimerA
    TA0CTL = MC_1;                              // Start TimerA0
    P1OUT ^= BIT0;                              // Turn on P1.6 (Red LED)
}
#pragma vector=TIMER0_A1_VECTOR                 // This interrupt is based off TIMER A1 and blinks the light at the given interval
__interrupt void Timer_A0(void)
{
    TA0CTL = MC_0;                              // Stop TimerA0
    TACTL = MC_1;                               // Start TimerA
    P1OUT &= ~BIT6;                             // Turn off P1.6 (Red LED)
}
#pragma vector = PORT1_VECTOR                   // Interrupt for button presses
__interrupt void Port_1(void)
{
if(P1IES & BIT3){                               // If button is pressed
if(TA0CCR0 == 0){                               // If TimerA0's counter is 0 then
   TACCR0 = 0;                                  // Reset TimerA CCR0 to zero 0%
   TA0CCR0 = 1000;                              // Reset TimerA0 CCR0 to max timer 100%
}else{
   TACCR0 += 100;                               // Add 10% to pulse
   TA0CCR0 -= 100;                              // Subtract 10% from pulse
}

}
P1IFG &= ~BIT3;                                 // Resets interrupt flag for P1.3
}
