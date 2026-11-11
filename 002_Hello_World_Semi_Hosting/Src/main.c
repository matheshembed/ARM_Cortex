#include <stdint.h>
#include <stdio.h>

extern void initialise_monitor_handles(void);

int main(void)
{
	initialise_monitor_handles();
	printf("Hello World\n");
	for(;;);
}
