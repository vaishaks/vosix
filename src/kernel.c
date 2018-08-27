#include "mini_uart.h"
#include "utils.h"
#include "spinlock.h"


void kernel_main(int cpuId)
{
	
	if (cpuId == 0) 
	{
		uart_init();
		uart_send_string("Booting into Vosix v0.01!\r\n");
	}

	char cpuIdStr[2];
	cpuIdStr[0] = cpuId + '0';
	cpuIdStr[1] = 0;

	try_lock(cpuId);
	uart_send_string("This is processor: ");
	uart_send_string(cpuIdStr);
	uart_send_string("\r\n");
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
