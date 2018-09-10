#include "mini_uart.h"
#include "utils.h"
#include "spinlock.h"
#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"

void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			delay(100000);
		}
		printf("\r\n");
	}
}

void kernel_main(int cpuId)
{
	
	if (cpuId == 0) 
	{
		int exec_lvl = get_el();
		uart_init(BAUD_115200);
		init_printf(0, putc);
		printf("\r\nBooting into Vosix v0.01!\r\n");
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
		delay(10000000);
		irq_vector_init();
		timer_init();
		enable_interrupt_controller();
		enable_irq();		

		int res = copy_process((unsigned long)&process, (unsigned long)"12345");
		if (res != 0) {
			printf("error while starting process 1");
			return;
		}
		res = copy_process((unsigned long)&process, (unsigned long)"abcde");
		if (res != 0) {
			printf("error while starting process 2");
			return;
		}

		while (1){
			schedule();
		}
	}	
	else
	{		
		while(1){}		
	}	
}
