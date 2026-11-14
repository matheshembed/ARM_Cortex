/*
Write a program to execute an SVC instruction from thread mode,
implement the svc handler to print the SVC number used.
Also,  increment the SVC number by 4 and return it to the thread mode code and print it.
Hints :
1)Write a main() function where you should execute the SVC instruction with an argument.
let's say SVC #0x5
2)Implement the SVC handler
3)In the SVC handler extract the SVC number and print it using printf
4) Increment the SVC number by 4 and return it to the thread mode
*/

#include <stdio.h>
#include <stdint.h>

int main(void)
{
	__asm volatile ("SVC #25"); // This causes SVC Exception

	//register uint32_t data __asm("r0");

	uint32_t data;

	__asm volatile ("MOV %0,R0": "=r"(data) ::);

	printf(" data = %ld\n",data);

	for(;;);
}

__attribute__ ((naked)) void SVC_Handler(void)
{
	//Extract the value of the SVC 0x08 (in this example)
	//1. Get the value of the MSP
	__asm volatile ("MRS R0,MSP");
	__asm volatile ("B SVC_Handler_C");

}

void SVC_Handler_C(uint32_t *pBaseofStackFrame)
{
	printf("SVC Exception Raised\n");
	printf("In SVC Handler\n");

	uint8_t *pReturnAddr = (uint8_t *) pBaseofStackFrame[6];

	//2. Decrement the return address by 2 to point to opcode of the SVC instruction in the  program memory
	pReturnAddr = pReturnAddr - 2 ;

	//3. Extract the SVC number (LSB of the opcode
	uint8_t svc_number = *pReturnAddr;
	printf("SVC Number is : %d",svc_number);

	svc_number += 4;

	pBaseofStackFrame[0] = svc_number;


}
