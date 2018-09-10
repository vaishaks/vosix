#include "mini_uart.h"
#include "utils.h"
#include "spinlock.h"
#include "printf.h"
#include "timer.h"
#include "irq.h"

void kernel_main(int cpuId)
{
	
	if (cpuId == 0) 
	{
		int exec_lvl = get_el();
		uart_init(BAUD_115200);
		init_printf(0, putc);
		printf("Booting into Vosix v0.01!\r\n");
		printf("Current exception level: %d\r\n", exec_lvl);		
	}

	char cpuIdStr[2];
	cpuIdStr[0] = cpuId + '0';
	cpuIdStr[1] = 0;

	try_lock(cpuId);
	printf("This is processor: %s\r\n", cpuIdStr);
	close_lock();
	
	if (cpuId == 0) 
	{
		irq_vector_init();
		timer_init();
		enable_interrupt_controller();
		enable_irq();		
		while (1) {
			uart_send(uart_recv());
		}
	}	
	else
	{		
		while(1){}		
	}	
}
