#include <stdint.h>
#include <stdio.h>

int main()
{
	//1. Enable all configurable exceptions like usage fault, mem manage fault and bus fault.
	/*
	 * Refer the Generic User Guide.
	 * Inside System Control Block -> System handler control and State register (0xE000ED24).
	 * We have to enable 18,17,16 th bit position for Usage,bus and Mem faults.
	 */
	uint32_t *pSHCSR = (uint32_t *) 0xE000ED24 ;

	*pSHCSR |= (1 << 16); // Mem Manage Fault.
	*pSHCSR |= (1 << 17); // Bus Fault.
	*pSHCSR |= (1 << 18); // Usage Fault.



	//3. Lets force the processor to execute some undefined instruction.
	uint32_t *pSRAM = (uint32_t *) 0x20010000; // Random address  lsb TO 1 to get the invalid state error and as it is for the 	undefined instruction
	*pSRAM = 0xFFFFFFFF; // Random Data (Invalid OP code)

	void (*some_address) (void);
	some_address = (void *)0x20010001; // T bit making it as odd (0th bit)
	some_address();

	//4. Analyze the fault.
	/*
	 * When a fault happens, inside the fault handler, you can check a couple of fault status and address information register to get more details
	 * about the fault and the instruction address at which the fault happened. This will be helpful during debugging.
	 * HFSR, MMFSR, BFSR, UFSR (Fault status registers) (and this everything is located in the SCB
	 */

}

//2. Implement the fault handlers.
void HardFault_Handler(void)
{
	printf("Exception: Hard Fault\t\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception: Bus Fault\t\n");
	while(1);
}

/*
 * When we use the Normal c code to see the stack .. The c compiler automatically stores the stack called epilogue
 * Our goal is to see as soon as the fault is raised since the compiler adds/did some manipulation to the stack we wont be able to see the proper stack frame that we wanted to see
 * That is why we are using this naked asm function (refer to the notes to get the idea on this)
 */
__attribute__ ((naked)) void UsageFault_Handler (void)
{
	//Extracted the value of MSP which happens to be the base address of the stack frame which got saved during the exception entry from thread mode to handler mode.
	//Stack Frame belongs to the Thread mode code.
	__asm ("MRS r0,MSP");
	__asm ("B UsageFault_Handler_c");

}

void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
	//This UFSR is inside the System Control Block SCB and its a 16 bit register.
	//There are bits in the UFSR to check which fault is raised

	uint32_t *pUFSR = (uint32_t*)0xE000ED2A;
	printf("Exception : UsageFault\n");
	printf("UFSR = %lx\n",(*pUFSR) & 0xFFFF);
	printf("pBaseStackFrame = %p\n",pBaseStackFrame);
	printf("Value of R0 = %lx\n", pBaseStackFrame[0]);
	printf("Value of R1 = %lx\n", pBaseStackFrame[1]);
	printf("Value of R2 = %lx\n", pBaseStackFrame[2]);
	printf("Value of R3 = %lx\n", pBaseStackFrame[3]);
	printf("Value of R12 = %lx\n", pBaseStackFrame[4]);
	printf("Value of LR = %lx\n", pBaseStackFrame[5]);
	printf("Value of PC = %lx\n", pBaseStackFrame[6]);
	printf("Value of XPSR = %lx\n", pBaseStackFrame[7]);
	while(1);

}

