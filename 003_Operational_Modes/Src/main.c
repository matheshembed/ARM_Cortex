#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF);

}

/* This function executes in THREAD MODE of the processor */
/*
 * As it said above this will execute in the THREAD MODE
 * But how do we verify that , In the Interrupt Program Status Register there is a field Called ISR_NUMBER for the status of the  THREAD MODE/HANDLER MODE status
 * If it is zero then That it is in THREAD MODE, If that is a non zero that is in the HANDLER MODE
 * Refer Generic User GUIDE
 */

/*
 * While Debugging See the XPSR(Program status register) in the registers view
 * see the bits from 0 to 8 which is the first 9 bits it will be zero and from that we can say that it is on the Thread Mode
 * Same way to see the Handler Mode (see the bits in this register
 */
int main(void)
{
	printf("In thread mode : before interrupt\n");

	generate_interrupt();

	printf("In thread mode : after interrupt\n");

	for(;;);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
	printf("In handler mode : ISR\n");
}
