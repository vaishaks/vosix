#include "mini_uart.h"
#include "utils.h"
#include "spinlock.h"
#include "printf.h"


void kernel_main(int cpuId)
{
	
	if (cpuId == 0) 
	{
		uart_init();
		init_printf(0, putc);
		printf("Booting into Vosix v0.01!\r\n");
	}

	char cpuIdStr[2];
	cpuIdStr[0] = cpuId + '0';
	cpuIdStr[1] = 0;

	try_lock(cpuId);
	printf("This is processor: %s\r\n", cpuIdStr);
	close_lock();
	
	if (cpuId == 0) 
	{
		while (1) {
			uart_send(uart_recv());
		}
	}	
	else
	{		
		while(1){}		
	}	
}
