#include <stdint.h>
#include <stdio.h>

#define ALIAS_BASE 0x22000000U
#define BIT_BAND_BASE 0x20000000U

int main(void)
{
	uint8_t *ptr = (uint8_t*)0x20000200;
	*ptr = 0xFF;

	/*
	 * Normal Method
	 */
	//Here we are clearing 7th Bit position.
	*ptr &= ~(1 << 7);

	//Reset to 0XFF
	*ptr = 0xFF;

	/*
	 * Bit Band Method
	 */
	//Alias Address
	uint8_t *alias_addr = (uint8_t*) (ALIAS_BASE + (32 * (0x20000200 - BIT_BAND_BASE)) + 7 * 4);

	//clearing 7th bit of address 0x20000200
	*alias_addr = 0;

	while(1);

}
