#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE + Privileged Access level of the processor */
/*
 * Since the thread mode is in unprivileged access level you cannot able to execute these instructions (in the generate interrupt)
 * 0xE000EF00,0xE000E100 These are some ARM Cortex M4 processor's systems control register addresses which can only be accessed in
 * privileged access level.
 *
 * Any attempt to change the contents of these registers from being in unprivileged access level will cause a processor fault exception.
 */
void generate_interrupt()
{
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF);

}

/*
 * To change the access level, you have to change something in the control register (Ref: Generic User Guide)
 * Make nPRIV = 1 to make as unprivileged
 * As you saw earlier, this is a core register this is not a memory mapped register this does not come with its own memory mapped address
 * you cannot use your c pointer to access these registers. Only Pure asm instruction to access this.
 */

void change_access_level_unpriv(void)
{
	/*read*/
	__asm volatile ("MRS R0,CONTROL");

	/*write*/
	__asm volatile ("ORR R0,R0,#0x01");
	//ORR -> Logical OR
	//ORR instruction (Data Manipulation and Processing instruction Section In generic Guide)
	//The destination is R0 (we are basically masking here) RO with immediate value 0x01

	//MSR to write back
	//This is the place where we are changing the access (priv -> unpriv)
	__asm volatile ("MSR CONTROL,R0");

	/*modify*/

}

int main(void)
{
	printf("In thread mode : before interrupt\n");

	change_access_level_unpriv();

	generate_interrupt();

	printf("In thread mode : after interrupt\n");

	for(;;);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
	printf("In handler mode : ISR\n");
}

void HardFault_Handler(void)
{
	printf("Hard Fault Detected");
	for (;;);
}
